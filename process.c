#include "headers.h"

struct msgbuff message;
/* Modify this file as needed*/
int remainingtime;

void Down(int msgq_id){
    //message queue to recieve the message from the server
    msgrcv(msgq_id, &message, sizeof(message.mRemainingTime), message.mtype, !IPC_NOWAIT);
    //printf("%s\n",message.mRemainingTime);
}

void decrementRemainingTime(int signum);

int main(int agrc, char *argv[])
{
    initClk();
    signal(SIGUSR1,decrementRemainingTime);
    int recieveRunningTime;
    message.mtype = getpid();
    recieveRunningTime = msgget(2, 0666 | IPC_CREAT);//message queue to recieve message from the server
    Down(recieveRunningTime);
    printf("inside process\n");
    
    printf("my remaining time = %d\n",message.mRemainingTime);
    remainingtime = message.mRemainingTime;

    //TODO The process needs to get the remaining time from somewhere
    //remainingtime = ??;
    while (remainingtime > 0)
    {
        // remainingtime = ??;
    }
    
    destroyClk(false);

    return 0;
}
void decrementRemainingTime(int signum){
    remainingtime--;
}