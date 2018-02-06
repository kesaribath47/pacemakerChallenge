#include "mbed.h"
#include "TextLCD.h"
#include "rtos.h"

RawSerial pc(USBTX, USBRX); // tx, rx

InterruptIn aget(p26);
InterruptIn vget(p27);
InterruptIn reset(p21);
DigitalOut apace(p7);
DigitalOut vpace(p8);

TextLCD lcd(p15, p16, p17, p18, p19, p20, TextLCD::LCD20x4);

//normal = 0, sleep = 1, exercise = 2;
int md = 0;
enum mode 
{
    NORMALMODE=0, SLEEPMODE, EXERCISEMODE, 
    MANUALMODE
};

enum State
{
    LRI=0, AS_received, PaceAMan,                       //PaceA
    PaceVInit, waitURI, AVI, PaceVMan, PAV_1, PAV_2,    //PaceV
    Sense1Init, PVARP,                                  //Sense1
    Sense2Init, VRP, AVdelay,                           //Sense2
    AVDelayInit, AVD1, AVD2,                            //AV_Delay_Calculation
    SensedAVDInit,                                      //Sensed_AV_Delay_offset
    ExPVARPInit, Exp1, Exp2, Exp3,                      //Ex_PVARP
};

int HeartRateLower[4] = {40,30,100,30};
int HeartRateUpper[4] = {100,60,175,175};

int TLRI[4]={1500, 2000, 600, 2000};
int TURI[4]={600, 1000, 343, 343};
 
int TAVI_l=30;
int TVRP_l=150;
int TPVARP_l=100;

int TAVI_u=100;
int TVRP_u=500;
int TPVARP_u=500;

//------------------Extra Credit
int PAV_max = 100;
int PAV_min = 30;
int PAV = 40;
//---------------------------


Timer tpa, tpv, clk, dispClk;
Timer master;
long timestamp = 0;
int lastDispUpdate=0;

int interval = 10;
int beats = 0;
int prev_beats = 0;


int fAS_PaceA = 0;
int fAS_PaceV = 0; 
int fAS_led = 0;

int fAP_PaceV = 0;
int fAP_led = 0;

int fVP_PaceA = 0;
int fVP_Sense1 = 0;
int fVP_Sense2 = 0;
int fVP_led = 0;

int fVS_PaceA = 0;
int fVS_PaceV = 0;
int fVS_Sense1 = 0;
int fVS_led = 0;

int freset_PaceA = 0;
int freset_PaceV = 0;
int freset_Sense1 = 0;
int freset_Sense2 = 0;
int freset_AVDelay = 0;
int freset_ExPVARP = 0;


int fmanualShift_PaceA = 0; 
int fmanualShift_PaceV = 0;
int fmanualShift_keyboard = 0;

int fAget_Sense1 = 0;

int fVget_Sense2 = 0;

int fAS_AVDelay = 0;
int fAP_AVDelay = 0;
int fVS_AVDelay = 0;
int fVP_AVDelay = 0;

int fAS_ExPVARP = 0;
int fAP_ExPVARP = 0;
int fVS_ExPVARP = 0;
int fVP_ExPVARP = 0;
int fex_PVARP = 0;

int fAS_SensedAVD = 0;




Mutex printBlock;

class automata
{
    public:
        State myState;
        void init(State st)
        {
            myState = st;
        }
        void tran(State target) 
        { 
            myState = target; 
        }
};
automata PaceA, PaceV, Sense1, Sense2, AVDelay, ExPVARP, SensedAVD;

void pulseApace() {
    apace = 1;
    wait_us(20);
    apace = 0;
}

void pulseVpace() {
    vpace = 1;
    wait_us(20);
    vpace = 0;
}

void fnPaceA()
{
    PaceA.init(LRI);
    tpa.reset();
    while(true)
    {
        switch(PaceA.myState)
        {
            case LRI:
                if(tpa.read_ms()>=(TLRI[md] - TAVI_u))
                {
                    /*
                    All AP!
                    */
                    fAP_led = 1;
                    fAP_PaceV = 1;
                    fAP_AVDelay = 1;
                    fAP_ExPVARP = 1;
                    int val = tpa.read_ms();
                    
                    pulseApace();
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      APace!      Time Bound: %d      Actual Bound: %d \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000, val, (TLRI[md] - TAVI_u));
                    printBlock.unlock();
                    
                    tpa.reset();
                    
                }
                else if(fAS_PaceA==1)
                {
                    fAS_PaceA = 0;
                    PaceA.tran(AS_received);
                }
                else if(fVP_PaceA==1)
                {
                    fVP_PaceA = 0;
                    tpa.reset();
                }
                else if(fVS_PaceA==1)
                {
                    fVS_PaceA = 0;
                    tpa.reset();
                }
                else if(freset_PaceA==1)
                {
                    freset_PaceA = 0;    
                    tpa.reset();
                }
                else if(fmanualShift_PaceA==1)
                {
                    fmanualShift_PaceA = 0;
                    PaceA.tran(PaceAMan);
                }

            break;
            
            
            case AS_received:
                if(freset_PaceA==1)
                {
                    freset_PaceA = 0;
                    tpa.reset();
                    PaceA.tran(LRI);
                }
                else if(fVP_PaceA==1)
                {
                    fVP_PaceA = 0;
                    PaceA.tran(LRI);
                    tpa.reset();
                }
                else if(fVS_PaceA==1)
                {
                    fVS_PaceA = 0;
                    PaceA.tran(LRI);
                    tpa.reset();
                }
                else if(fmanualShift_PaceA==1)
                {
                    fmanualShift_PaceA = 0;
                    PaceA.tran(PaceAMan);
                }
            break;
                
                
            case PaceAMan:
                if(freset_PaceA==1)
                {
                    freset_PaceA = 0;
                    tpa.reset();
                    PaceA.tran(LRI);
                }
            break;
        }
        Thread::wait(1);
    }
}

void fnPaceV()
{
    PaceV.init(PaceVInit);
    tpv.reset();
    clk.reset();
    while(true)
    {
        switch(PaceV.myState)
        {
            case PaceVInit:
                if(fAS_PaceV==1)
                {
                    fAS_PaceV=0;
                    tpv.reset();
                    PaceV.tran(AVI);
                }
                if(fAP_PaceV==1)
                {
                    fAP_PaceV=0;
                    tpv.reset();
                    PaceV.tran(AVI);
                }
                else if(freset_PaceV==1)
                {
                    freset_PaceV=0;
                    tpv.reset();
                }
                else if(fmanualShift_PaceV==1)
                {
                    fmanualShift_PaceV = 0;
                    PaceV.tran(PaceVMan);
                }
            break;
            
            
            case PaceVMan:
                if(freset_PaceV==1)
                {
                    freset_PaceV = 0;
                    tpv.reset();
                    PaceV.tran(PaceVInit);
                }
            break;
            
            
            case AVI:
                if(fVS_PaceV==1)
                {
                    fVS_PaceV = 0;
                    
                    pc.printf("AVI -> Init 4\n\r");
                    PaceV.tran(PaceVInit);
                }
                else if(freset_PaceV==1)
                {
                    freset_PaceV = 0;
                    tpv.reset();
                    
                    pc.printf("AVI -> Init 1\n\r");
                    PaceV.tran(PaceVInit);
                }
                else if(fmanualShift_PaceV==1)
                {
                    fmanualShift_PaceV = 0;
                    PaceV.tran(PaceVMan);
                }
                else if((tpv.read_ms() >= TAVI_l) && (clk.read_ms() >= TURI[md]))
                {
                    

                    PaceV.tran(PAV_2);

                }
                else if((tpv.read_ms() >= TAVI_l) && (clk.read_ms() < TURI[md]))
                {
                    pc.printf("AVI -> URI 3\n\r");
                    PaceV.tran(waitURI);
                    int val = clk.read_ms();
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      Waiting for URI      Time Bound: %d      Actual Bound: %d \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000, val, TURI[md]);
                    printBlock.unlock();
                }
            break;
            
            
            case waitURI:
                if(freset_PaceV == 1)
                {
                    freset_PaceV = 0;
                    tpv.reset();
                    
                    PaceV.tran(PaceVInit);
                }
                else if(fmanualShift_PaceV==1)
                {
                    fmanualShift_PaceV = 0;
                    PaceV.tran(PaceVMan);
                }
                else if(fVS_PaceV == 1)
                {
                    fVS_PaceV = 0;
                    PaceV.tran(PaceVInit);
                }
             

                //------------Extra Credit------------------
                else if(clk.read_ms() >= TURI[md])
                {
                    PaceV.tran(PAV_1);
                }
            break;

            case PAV_1:
                if (freset_PaceV == 1)
                {
                    freset_PaceV = 0;
                    tpv.reset();
                    
                    PaceV.tran(PaceVInit);
                } 
                else if(fVS_PaceV == 1)
                {
                    fVS_PaceV = 0;
                    PaceV.tran(PaceVInit);
                }
                else if (tpv.read_ms() >= PAV)
                {
                    
                //     VP!
                    
                    fVP_Sense1 = 1;
                    fVP_Sense2 = 1;
                    fVP_led = 1;
                    fVP_PaceA = 1;
                    fVP_AVDelay = 1;
                    fVP_ExPVARP = 1;
                    beats++;
                    pulseVpace();

                    int val = tpv.read_ms();
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      VPace!      Time Bound: %d      Actual Bound: %d \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000, val, PAV);
                    printBlock.unlock();
                    
                    clk.reset();
                    PaceV.tran(PaceVInit);
                }
                break;

                case PAV_2:
                if (freset_PaceV == 1)
                {
                    freset_PaceV = 0;
                    tpv.reset();
                    
                    PaceV.tran(PaceVInit);
                } 
                else if (tpv.read_ms() >= PAV)
                {
                    
                //     VP!
                    
                    fVP_Sense1 = 1;
                    fVP_Sense2 = 1;
                    fVP_led = 1;
                    fVP_PaceA = 1;
                    fVP_AVDelay = 1;
                    fVP_ExPVARP = 1;
                    beats++;
                    pulseVpace();

                    int val = tpv.read_ms();
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      VPace!      Time Bound: %d      Actual Bound: %d \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000, val, PAV);
                    printBlock.unlock();
                    
                    clk.reset();
                    PaceV.tran(PaceVInit);
                }
                break;

        }

        Thread::wait(1);
    }
}

void fnSense1()
{
    Timer t;
    t.start();
    Sense1.init(Sense1Init);
    while(true)
    {
        switch(Sense1.myState)
        {
            case Sense1Init:
            
                if(fVS_Sense1 == 1)
                {
                    fVS_Sense1 = 0;
                    t.reset();
                    Sense1.tran(PVARP);
                }
                else if(fAget_Sense1 == 1)
                {
                    fAget_Sense1 = 0;
                    /*

                    AS!

                    */
                    fAS_led = 1;
                    fAS_PaceV = 1;
                    fAS_PaceA = 1;
                    fAS_AVDelay = 1;
                    fAS_ExPVARP = 1;
                    fAS_SensedAVD = 1;
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld   %d   Aget Received\n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000, fVS_PaceV);
                    printBlock.unlock();
                    
                }
                else if(freset_Sense1 == 1)
                {
                    freset_Sense1 = 0;
                    t.reset();
                }
                else if(fVP_Sense1 == 1)
                {
                    fVP_Sense1 = 0;
                    t.reset();
                    Sense1.tran(PVARP);
                }
            
            break;
            

            case PVARP:

                if(fAget_Sense1 == 1)
                {
                    fAget_Sense1 = 0;
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      Aget Ignored\n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                    printBlock.unlock();
                    
                }
                else if(freset_Sense1 == 1)
                {
                    freset_Sense1 = 0;
                    t.reset();
                    Sense1.tran(Sense1Init);
                }
                else if(t.read_ms() >= TPVARP_l)
                {
                    Sense1.tran(Sense1Init);
                }

            break;
        }
        Thread::wait(1);
    }
}

void fnSense2()
{
    Timer t;
    t.start();
    Sense2.init(Sense2Init);

    while(true)
    {
        switch(Sense2.myState)
        {
            case Sense2Init:
                if(freset_Sense2 == 1)
                {
                    freset_Sense2 = 0;
                    t.reset();
                }
                else if(fVget_Sense2 == 1)
                {
                    fVget_Sense2 = 0;
                    t.reset();
                    clk.reset();
                    /*

                    VS!

                    */
                    fVS_Sense1 = 1;
                    fVS_AVDelay = 1;
                    fVS_led = 1;
                    fVS_PaceV = 1;
                    fVS_PaceA = 1;
                    fVS_ExPVARP = 1;
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      Vget Received\n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                    printBlock.unlock();
                    
                    beats++;
                    Sense2.tran(VRP);
                }
                else if(fVP_Sense2 == 1)
                {
                    fVP_Sense2 = 0;
                    t.reset();
                    Sense2.tran(VRP);
                }
            break;

            case VRP:
                if((t.read_ms() <= TVRP_u) && fVget_Sense2 == 1)
                {
                    fVget_Sense2 = 0;
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      Vget Ignored\n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);                   
                    printBlock.unlock();
                    
                }
                else if(t.read_ms() >= TVRP_l)
                {
                    Sense2.tran(Sense2Init);
                }
                else if(freset_Sense2 == 1)
                {
                    freset_Sense2 = 0;
                    t.reset();
                    Sense2.tran(Sense2Init);
                }
            break;
        }
        Thread::wait(1);
    }

}

void Rx_interrupt () {
    if(pc.readable())
    {
        char t = pc.getc();
        char command[3];
        switch(t)
        {
            case 'N': 
                md=0; 
                fmanualShift_keyboard = 0;
                
                printBlock.lock();
                timestamp = master.read_ms();
                pc.printf("\n\r %ld:%ld:%ld      Normal Mode \n\r \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                printBlock.unlock();
                lastDispUpdate=dispClk.read(); 
                beats = 0;
                
                tpa.reset();
                tpv.reset();
                clk.reset();
                freset_Sense2 = 1;
                freset_Sense1 = 1;
                freset_PaceV = 1;
                freset_PaceA = 1;
                freset_AVDelay = 1;
                freset_ExPVARP = 1;
                break;
            case 'S': 
                md=1;
                fmanualShift_keyboard = 0;

                printBlock.lock();
                timestamp = master.read_ms();
                pc.printf("\n\r %ld:%ld:%ld      Sleep Mode \n\r \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                printBlock.unlock();
                lastDispUpdate=dispClk.read(); 
                beats = 0;
                tpa.reset();
                tpv.reset();
                clk.reset();
                freset_Sense2 = 1;
                freset_Sense1 = 1;
                freset_PaceV = 1;
                freset_PaceA = 1;
                freset_AVDelay = 1;
                freset_ExPVARP = 1;
                break;
            case 'E': 
                md=2;
                fmanualShift_keyboard = 0;
                
                printBlock.lock();
                timestamp = master.read_ms();
                pc.printf("\n\r %ld:%ld:%ld      Exercise Mode \n\r \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                printBlock.unlock();
                lastDispUpdate=dispClk.read(); 
                beats = 0;

                tpa.reset();
                tpv.reset();
                clk.reset();
                freset_Sense2 = 1;
                freset_Sense1 = 1;
                freset_PaceV = 1;
                freset_PaceA = 1;
                freset_AVDelay = 1;
                freset_ExPVARP = 1;
                break;
            case 'M':
                fmanualShift_PaceV = 1;
                fmanualShift_PaceA = 1;
                fmanualShift_keyboard = 1;
                // manualShift!

                freset_Sense2 = 1;
                freset_Sense1 = 1;
                freset_PaceV = 1;
                freset_PaceA = 1;
                freset_AVDelay = 1;
                freset_ExPVARP = 1;
                // reset!
                printBlock.lock();
                timestamp = master.read_ms();
                pc.printf("\n\r %ld:%ld:%ld      Manual Mode \n\r \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                printBlock.unlock();
                lastDispUpdate=dispClk.read(); 
                beats = 0;
                
                tpa.reset();
                tpv.reset();
                clk.reset();
                break;
            case 'A': 
                if(fmanualShift_keyboard == 1) {
                    fAP_PaceV = 1;
                    fAP_AVDelay = 1;
                    fAP_led = 1;
                    fAP_ExPVARP = 1;
                    pulseApace();
                
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      Manual APace \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                    printBlock.unlock();
                    // AP!
                }
                
                break;
            case 'V': 
                if(fmanualShift_keyboard == 1) {
                    fVP_PaceA = 1;
                    fVP_led = 1;
                    fVP_Sense1 = 1;
                    fVP_Sense2 = 1;
                    fVP_AVDelay = 1;
                    fVP_ExPVARP = 1;
                    beats++;
                    pulseVpace();
                    
                    printBlock.lock();
                    timestamp = master.read_ms();
                    pc.printf("%ld:%ld:%ld      Manual VPace \n\r",timestamp/60000, (timestamp/1000)%60, timestamp%1000);
                    printBlock.unlock();
                    // VP!
                }
                break;
            case 'O': 
                int i = 0;
                
                printBlock.lock();
                pc.printf("\n\r Set Observation Interval : ");
                printBlock.unlock();    
                while(t!='\r')
                {
                    t = pc.getc();
                    command[i++] = t;    
                }
                interval = atoi(command);
                
                printBlock.lock();
                timestamp = master.read_ms();
                pc.printf("%d \n\r",interval);
                printBlock.unlock();
                lastDispUpdate=dispClk.read(); 
                beats = 0;
                
                break;
        }
    }
}

void aget_isr() {
    fAget_Sense1 = 1;
}

void vget_isr() {
    fVget_Sense2 = 1;
}

void reset_isr() {
    freset_Sense2 = 1;
    freset_Sense1 = 1;
    freset_PaceV = 1;
    freset_PaceA = 1;
    freset_AVDelay = 1;
    freset_ExPVARP = 1;
}

void led_thread(void const *args) 
{
    DigitalOut led1(LED1);  //AS
    DigitalOut led2(LED2);  //Apace
    DigitalOut led3(LED3);  //VS
    DigitalOut led4(LED4);  //Vpace
    
    while(1)
    {
        if(fAS_led) {
            fAS_led = 0;
            led1 = 1;
        }
        if(fVS_led) {
            fVS_led = 0;
            led3 = 1;
        }
        if(fAP_led) {
            fAP_led = 0;
            led2 = 1;
        }
        if(fVP_led) {
            fVP_led = 0;
            led4 = 1;
        }
        if(led1 == 1 || led2 == 1 || led3 == 1 || led4 == 1)
            wait_ms(50); 
        led1 = 0;led2 = 0; led3 = 0; led4 = 0;
        
        Thread::wait(1);
    }
}

void checkAlarm(int rate)
{
    if(rate<HeartRateLower[md])
    {
        lcd.locate(0,1);
        lcd.printf("LOW RATE ALARM!");
    }
    else if(rate>HeartRateUpper[md])
    {
        lcd.locate(0,1);
        lcd.printf("HIGH RATE ALARM!");
    }
    else
    {
        lcd.locate(0,1);
        lcd.printf("                ");
    }
}

void display_thread(void const *args)
{
    dispClk.start();
    while(1)
    {
        //pc.printf("%f\r\n",(float)dispClk.read()-lastDispUpdate);
        if((dispClk.read()-lastDispUpdate) > interval)
        {
            int rate = (int)(60*(float)beats/(float)interval);
            lcd.locate(0,0);
            lcd.printf("bpm:    %3d   ",rate);
            checkAlarm(rate);
            beats = 0;
            lastDispUpdate = dispClk.read();
            
            printBlock.lock();
            pc.printf("\n\rDisplay Updated \n\r");
            pc.printf("states : %d %d %d %d %d\n\r",PaceA.myState, PaceV.myState, Sense1.myState, Sense2.myState, fAS_PaceV);
            printBlock.unlock();

        }
        wait(1);
    }    
}

void fnAVDCalc()
{
    Timer t;
    t.start();
    int beats1 = 0;
    AVDelay.init(AVDelayInit);
    while(true)
    {
        switch(AVDelay.myState)
        {
            case AVDelayInit:
                if(fAS_AVDelay)
                {
                    fAS_AVDelay = 0;
                    AVDelay.tran(AVD1);
                    t.reset();
                }
                else if(fAP_AVDelay)
                {
                    fAP_AVDelay = 0;
                    AVDelay.tran(AVD1);
                    t.reset();
                }
            break;

            case AVD1:
                if(fVP_AVDelay)
                {
                    fVP_AVDelay = 0;
                    AVDelay.tran(AVD2);
                }
                else if(fVS_AVDelay)
                {
                    fVS_AVDelay = 0;
                    AVDelay.tran(AVD2);
                }
            break;

            case AVD2:
                beats1 = t.read_ms();
                printBlock.lock();
                pc.printf("Old PAV = %d\n\r",PAV);
                printBlock.unlock();
                if(prev_beats > t.read_ms())
                {
                    PAV=PAV+4;
                }
                else if (prev_beats < t.read_ms())
                {
                    PAV=PAV-4;
                }
                if(PAV<PAV_min)
                {
                    PAV=PAV_min;
                }
                if(PAV>PAV_max)
                {
                    PAV=PAV_max;
                }

                printBlock.lock();
                pc.printf("PAV = %d, Rate difference = %d \n\r",PAV, prev_beats-beats1);
                printBlock.unlock();

                prev_beats=t.read_ms();
                AVDelay.tran(AVDelayInit);
                break;

        }
        Thread::wait(1);  
    }
    
}

void fnExPVARP()
{
    int l = 100;
    int u = 500;
    Timer t;
    t.start();
    ExPVARP.init(ExPVARPInit);
    while(1)
    {
        switch(ExPVARP.myState)
        {
            case ExPVARPInit:
            if(fAP_ExPVARP)
            {
                fAP_ExPVARP = 0;
                t.reset();
                ExPVARP.tran(Exp1);
            }
            else if(fAS_ExPVARP)
            {
                fAS_ExPVARP = 0;
                t.reset();
                ExPVARP.tran(Exp1);
            }
            break;

            case Exp1:
            if(freset_ExPVARP)
            {
                freset_ExPVARP = 0;
                ExPVARP.tran(ExPVARPInit);
            }
            else if(t.read_ms() >= TAVI_l)
            {
                ExPVARP.tran(ExPVARPInit);
            }
            else if(fVS_ExPVARP)
            {
                fVS_ExPVARP = 0;
                ExPVARP.tran(Exp2);
                TPVARP_l = l + 20;
                printBlock.lock();
                pc.printf("PVC Occured (Time Bound: %d, Actual Bound: %d)\n \r",t.read_ms(),TAVI_l);
                pc.printf("New TPVARP: %d \n\r",TPVARP_l);
                printBlock.unlock();
            }
            
            break;

            case Exp2:
            fex_PVARP = 1;
            ExPVARP.tran(Exp3);

            break;

            case Exp3:
            if (fVS_ExPVARP)
            {
                fVS_ExPVARP = 0;
                TPVARP_l = l;
                ExPVARP.tran(ExPVARPInit);
                printBlock.lock();
                pc.printf("New TPVARP = %d (Restored)\n\r",TPVARP_l);
                printBlock.unlock();
            }
            else if (fVP_ExPVARP)
            {
                fVP_ExPVARP = 0;
                TPVARP_l = l;
                ExPVARP.tran(ExPVARPInit);
                printBlock.lock();
                pc.printf("New TPVARP = %d (Restored)\n\r",TPVARP_l);
                printBlock.unlock();
            }
            else if (fAP_ExPVARP)
            {
                fAP_ExPVARP = 0;
                TPVARP_l = l;
                ExPVARP.tran(ExPVARPInit);
                printBlock.lock();
                pc.printf("New TPVARP = %d (Restored)\n\r",TPVARP_l);
                printBlock.unlock();
            }
            else if (fAS_ExPVARP)
            {
                fAS_ExPVARP = 0;
                TPVARP_l = l;
                ExPVARP.tran(ExPVARPInit);
                printBlock.lock();
                pc.printf("New TPVARP = %d (Restored)\n\r",TPVARP_l);
                printBlock.unlock();
            }
            else if (freset_ExPVARP)
            {
                freset_ExPVARP = 0;
                ExPVARP.tran(ExPVARPInit);
            }
            break;
        }
        Thread::wait(1);
    }     
}

void fnSensedAVD()
{
    SensedAVD.init(SensedAVDInit);
    while(1)
    {
        if(fAS_SensedAVD)
        {
            fAS_SensedAVD = 0;
            PAV = PAV - 4;
            if (PAV < PAV_min)
            {
                PAV = PAV_min;
            }

            if (PAV > PAV_max)
            {
                PAV = PAV_max;
            }
            printBlock.lock();
            pc.printf("PAV_Sensed = %d \n\r",PAV);
            printBlock.unlock();
        }
        Thread::wait(1);
    }
}

int main() 
{
    pc.baud(115200);
    pc.attach(&Rx_interrupt, Serial::RxIrq);
    aget.rise(&aget_isr);
    vget.rise(&vget_isr);
    reset.rise(&reset_isr);

    Thread PaceA_thread(fnPaceA);
    Thread PaceV_thread(fnPaceV);    
    Thread Sense1_thread(fnSense1);    
    Thread Sense2_thread(fnSense2);
    Thread AV_Delay_Calculation_Thread(fnAVDCalc);
    Thread Ex_PVARP_Thread(fnExPVARP);
    Thread Sensed_AVD_Thread(fnSensedAVD);
    Thread led(led_thread);
    Thread display(display_thread);  
    
    PaceA_thread.set_priority(osPriorityAboveNormal);
    PaceV_thread.set_priority(osPriorityAboveNormal);
    
    tpa.start();
    tpv.start();
    clk.start();
    master.start();
    
    
    while(1) 
    {
        
 

    }
}