//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*

*/
E<> ProcMonitorPVARP.three imply ProcMonitorPVARP.t>TPVARP[0]

/*

*/
A[] ProcMonitorPVARP.three imply ProcMonitorPVARP.t>TPVARP[0]

/*

*/
E<> ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[1]

/*

*/
A[] ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[2]

/*

*/
A[] ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[1]

/*

*/
A[] ProcMonitorURI.three imply ProcMonitorURI.t>=TURI[0]

/*

*/
A[] ProcMonitorLRI.three imply ProcMonitorLRI.t<=TLRI[2]

/*

*/
A[] ProcMonitorLRI.three imply ProcMonitorLRI.t<=TLRI[1]

/*

*/
A[] ProcMonitorLRI.three imply ProcMonitorLRI.t<=TLRI[0]

/*
A[] ProcMonitorPVARP.three imply ProcMonitorPVARP.t>TPVARP[0]
*/
A[] not deadlock\

