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

int msgq_idUp;
int msgq_idDown;
void handler(int signum);

typedef struct msgbuff
{
    long mtype;
    int mRemainingTime;
    
}msgbuff;

struct msgbuff message;

void Down(int msgq_id){
    //used to send the reversed message
    //printf("reversed msg by server = %s\n",message.mRemainingTime);
    msgsnd(msgq_id, &message, sizeof(message.mRemainingTime), !IPC_NOWAIT);
    //strcpy(message.mRemainingTime,"");
    
}

int main(int argc, char **argv){
    key_t key_id;
    int rec_val;
    //signal(SIGINT,handler);
   
    message.mtype = 5;
    message.mRemainingTime =20;
    msgq_idDown = msgget(2, 0666 | IPC_CREAT);

    Down(msgq_idDown);//sleep(1);


}

void handler(int signum){
    msgctl(msgq_idUp,IPC_RMID,(struct msqid_ds*)0);
    msgctl(msgq_idDown,IPC_RMID,(struct msqid_ds*)0);
    printf("\n");
    exit(-1);
}