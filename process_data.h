#ifndef ProcessData_h
#define ProcessData_h



typedef struct ProcessData
{   
    int pid;
    int arrivalTime;
    int priority;
    int runningTime;
    int remainingTime;
    int status;
    //int waitingTime;
    //int startTime;
    //int finishTime;
    //int memorySize;
    int id;
}ProcessData;


#endif