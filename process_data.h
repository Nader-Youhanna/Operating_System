#ifndef ProcessData_h
#define ProcessData_h

enum State {
    started,
    resumed,
    stopped,
    finished
};

typedef struct ProcessData
{   
    int logTime;
    int pid;
    int arrivalTime;
    int priority;
    int runningTime;
    int remainingTime;
    enum State status;
    int waitingTime;
    int startTime;
    int finishTime;
    float weightedTA;
    //int memorySize;
    int id;
}ProcessData;





#endif