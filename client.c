#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/msg.h>

struct msgbuff
{
    long mtype;
    int mRemainingTime;
    
};

struct msgbuff message;


// void Up(int msgq_id){
//     //message queue to send the message to the server
//     msgsnd(msgq_id, &message, sizeof(message.mRemainingTime), !IPC_NOWAIT);
//     //printf("sent message by client = %s\n",message.mRemainingTime);
// }

void Down(int msgq_id){
    //message queue to recieve the message from the server
    msgrcv(msgq_id, &message, sizeof(message.mRemainingTime), message.mtype, !IPC_NOWAIT);
    //printf("%s\n",message.mRemainingTime);
}


int main(int argc, char **argv){
    int sendRunningTime;

    sendRunningTime = msgget(2, 0666 | IPC_CREAT);//message queue to recieve message from the server

    message.mtype = 5;
 
    Down(sendRunningTime);
    printf("%d\n",message.mRemainingTime);


}
