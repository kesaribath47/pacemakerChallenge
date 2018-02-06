//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*
Liveliness check for Testing Mode of Heart: Checks whether the heart enters Test mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessHeart.TestMode

/*
Liveliness check for Random Mode of Heart: Checks whether the heart enters random mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessHeart.RandomMode

/*
Liveliness check for Manual Mode of Heart: Checks whether the heart enters manual mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessHeart.ManualMode

/*
Liveliness check for Manual Mode of Pacemaker simulating a manual generation of VPace: Checks whether the Pacemaker enters Manual mode and simulates a VPace generation
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessPmodes.V

/*
Liveliness check for Manual Mode of Pacemaker simulating a manual generation of APace: Checks whether the Pacemaker enters Manual mode and simulates a APace generation
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessPmodes.A

/*
Liveliness check for Manual Mode of Pacemaker: Checks whether the Pacemaker enters Manual mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessPmodes.Manual

/*
Liveliness check for Exercise Mode of Pacemaker: Checks whether the Pacemaker enters Exercise mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> md==2

/*
Liveliness check for Sleep Mode of Pacemaker: Checks whether the Pacemaker enters Sleep mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> md==1

/*
Liveliness check for Normal Mode of Pacemaker: Checks whether the Pacemaker enters Normal mode
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> md==0

/*
Liveliness check for AGet LED of heart: Checks whether the heart enables AGet LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Heart, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessLED_Heart.AGet

/*
Liveliness check for VGet LED of heart: Checks whether the heart enables VGet LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Heart, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessLED_Heart.VGet

/*
Liveliness check for APace LED of heart: Checks whether the heart enables APace LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Heart, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessLED_Heart.APace

/*
Liveliness check for VPace LED of heart: Checks whether the heart enables VPace LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Heart, ProcessKeyboard_heart, Alarm;
*/
E<> ProcessLED_Heart.VPace

/*
Liveliness check for ASense LED of Pacemaker: Checks whether the heart enables ASense LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessLED_Pacemaker.ASense

/*
Liveliness check for VSense LED of Pacemaker: Checks whether the heart enables VSense LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessLED_Pacemaker.VSense

/*
Liveliness check for APace LED of Pacemaker: Checks whether the heart enables APace LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessLED_Pacemaker.APace

/*
Liveliness check for VPace LED of Pacemaker: Checks whether the heart enables VPace LED
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, Alarm;
*/
E<> ProcessLED_Pacemaker.VPace

/*
Liveliness check for Display of Pacemaker: Checks whether the pacemaker Display is active
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Heart, ProcessHeart_Pace, ProcessHeartRate, Alarm;
*/
E<> ProcessHeartRate.Display

/*
Liveliness check for Display of Pacemaker: Checks whether the pacemaker Display is active
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, ProcMonitorLRI, Alarm;
*/
E<> ProcMonitorLRI.Display

/*
Liveliness check for HighAlarm of Pacemaker: Checks whether the pacemaker High heartrate alarm is active
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, ProcMonitorLRI, Alarm;
This property will not be satisfied by default because the pacemaker performs within the expected range.
*/
E<> ProcessAlarm.HighAlarm

/*
Liveliness check for LowAlarm of Pacemaker: Checks whether the pacemaker Low heartrate alarm is active
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_Pace, ProcMonitorLRI, Alarm;
This property will not be satisfied by default because the pacemaker performs within the expected range.
*/
E<> ProcessAlarm.LowAlarm

/*
Testing Mode of Heart : Query checks the PVARP functionality by providing heart signals and checking time interval between consecutive VS\/VP and AS\/AP.
You can change the pacemaker mode using "md" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
A[] ProcHMonitorPVARP.PVARP imply ProcHMonitorPVARP.t>=TPVARP_l

/*
Testing Mode of Heart : Query checks the AVI functionality by providing heart signals and checking time interval between consecutive AS\/AP and VS\/VP.
You can change the pacemaker mode using "md" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
A[] ProcHMonitorAVI.AVI imply ProcHMonitorAVI.t>=TAVI_l

/*
Testing Mode of Heart : Query checks the VRP functionality by providing heart signals and checking consecutive VS\/VP and time interval between them.
You can change the pacemaker mode using "md" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
A[] ProcHMonitorPVRP.VRP imply ProcHMonitorPVRP.t>=TVRP_l

/*
Testing Mode of Heart : URI avoids pacing the heart too fast. Query checks if VP provided by pacemaker is always distant by URI interval after previous ventricular event.(VS,VP)
You can change the pacemaker mode using "md" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
A[] ProcHMonitorURI.URI imply ProcHMonitorURI.t>=TURI[md]

/*
Testing Mode of Heart : Checking the functionality of LRI operation as per the definition : If no atrial event has been sensed (AS), the component will deliver atrial pacing (AP) after TLRI-TAVI.
You can change the pacemaker mode using "md" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcessKeyboard_heart, Alarm;
*/
A[] ProcHMonitorLRI.LRI imply ProcHMonitorLRI.t<=TLRI[md]-TAVI_u

/*
Safety propert for mainitaing lower boud of AVI timing cycle: Checks whether the pacemaker always adhears to the lower limit of the AVI timing cycle
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorAVI.three imply ProcMonitorAVI.t>=TAVI_l

/*
Safety propert for mainitaing lower boud of PVRP timing cycle: Checks whether the pacemaker always adhears to the lower limit of the PVRP timing cycle
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorPVRP.three imply ProcMonitorPVRP.t>=TVRP_l

/*
Safety propert for mainitaing lower boud of PVARP timing cycle: Checks whether the pacemaker always adhears to the lower limit of the PVARP timing cycle
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorPVARP.three imply ProcMonitorPVARP.t>=TPVARP_l

/*
Safety propert for mainitaing lower boud of URI timing cycle for exercise mode: Checks whether the pacemaker always adhears to the lower limit of the URI timing cycle in exercise mode
You should change the pacemaker mode to "md = 2" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[2]

/*
Safety propert for mainitaing lower boud of URI timing cycle for sleep mode: Checks whether the pacemaker always adhears to the lower limit of the URI timing cycle in sleep mode
You should change the pacemaker mode to "md = 1" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[1]

/*
Safety propert for mainitaing lower boud of URI timing cycle for normal mode: Checks whether the pacemaker always adhears to the lower limit of the URI timing cycle in normal mode
You should change the pacemaker mode to "md = 0" variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[0]

/*
Safety propert for mainitaing upper boud of LRI timing cycle for exercise mode: Checks whether the pacemaker always adhears to the upper limit of the LRI timing cycle in exercise mode
You should change the pacemaker mode to "md =2 " variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorLRI.three imply ProcMonitorLRI.t<=TLRI[2]

/*
Safety propert for mainitaing upper boud of LRI timing cycle for sleep mode: Checks whether the pacemaker always adhears to the upper limit of the LRI timing cycle in sleep mode
You should change the pacemaker mode to "md =1 " variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorLRI.three imply ProcMonitorLRI.t<=TLRI[1]

/*
Safety propert for mainitaing upper boud of LRI timing cycle for normal mode: Checks whether the pacemaker always adhears to the upper limit of the LRI timing cycle in normal mode
You should change the pacemaker mode to "md =0 " variable in declarations
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessLED_Pacemaker, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] ProcMonitorLRI.three imply ProcMonitorLRI.t<=TLRI[0]

/*
Safety property of the entire system
In the system declarations, templates to be disabled section, uncomment following templates :
ProcessKeyboard_Pace, ProcMonitorLRI, ProcMonitorURI, ProcMonitorAVI, ProcMonitorPVARP, ProcMonitorPVRP, Alarm;
*/
A[] not deadlock
