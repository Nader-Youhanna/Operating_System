#include "headers.h"
#include "Queue.h"

struct msgbuff message;
ProcessData prev;
ProcessData temp;
//prev.id = 0;
bool reading = true;
float CPU_utilization;
int processNum=0;
float totalweightedTurnAround = 0;
float avgweightedTurnAround = 0;
int totalWaitingTime=0;
float avgWaitingTime=0;
int usefulRunTime = 0;
Queue* PCB;
void Down(int msgq_id){
    //used to send the reversed message
    msgsnd(msgq_id, &message, sizeof(message.mRemainingTime), !IPC_NOWAIT); 
}

void FCFS(int sem1, int sem2, int algorithm, ProcessData* shmaddr);
/* arg for semctl system calls. */
union Semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    ushort *array;         /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    PCB = (Queue*) malloc(sizeof(Queue*));
    initClk();

    int algorithm;
    int shmidAlgo;
    shmidAlgo = shmget(9, 4096, IPC_CREAT | 0644);
    if (shmidAlgo == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    int *shmaddrAlgorithm = shmat(shmidAlgo, (void *)0, 0);
    if(shmaddrAlgorithm == (void*) -1)	
    {
        perror("Error in attaching\n");
        exit(-1);
    }
    //*shmaddrAlgorithm = quantum;
    algorithm = *shmaddrAlgorithm;
    printf("algo = %d\n",algorithm);

    int shmid;
    shmid = shmget(10, 4096, IPC_CREAT | 0644);
    if (shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    ProcessData *shmaddr = shmat(shmid, (void *)0, 0);
    if(shmaddr == (void*) -1)	
    {
        perror("Error in attaching\n");
        exit(-1);
    }

    //schedulerType = *shmaddr;

    union Semun semun;
    int sem1 = semget(2, 1, 0666 | IPC_CREAT);
    int sem2 = semget(3, 1, 0666 | IPC_CREAT);
    if (sem1 == -1 || sem2 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }
    semun.val = 0; /* initial value of the semaphore, Binary semaphore */
    if (semctl(sem1, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
    if (semctl(sem2, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
    //TODO: implement the scheduler.


    if(algorithm==1){
        FCFS(sem1, sem2, algorithm,shmaddr);
    }

    
    //TODO: upon termination release the clock resources.
    destroyClk(true);
}

void FCFS(int sem1, int sem2, int algorithm, ProcessData* shmaddr){
    Queue *readyQueue = (Queue*) malloc(sizeof(Queue*));
    char buf[50];
    getcwd(buf,sizeof(buf));
    ProcessData runningProcess;
    runningProcess.remainingTime = 0;
    int prevTime = 0;
    int currentTime;

    while(1){

        // prevTime = currentTime;
        // currentTime = getClk();
        if(1){
            //printf("Current Time is %d\n", currentTime);
            if(reading){
                
                down(sem1);
                
                
                temp = *shmaddr;
                //printf("%d\n",temp.id);
                if(temp.id != -1 ){
                    if(temp.id!=prev.id && temp.arrivalTime >0){
                        //printf("%d\n",temp.id);
                        printf("%d %d %d\n",temp.arrivalTime,temp.runningTime,temp.priority);
                        processNum++;
                        usefulRunTime += temp.runningTime;
                        int processPid = fork();
                        if(processPid!=0){
                        
                        temp.pid = processPid;
                        enqueue(readyQueue,temp);
                        printf("%d\n",temp.pid); 
                        }
                        if(processPid == 0){
                            //temp.pid = getpid();
                            char buffer[265];
                            //itoa(temp.runningTime,buffer,10); 
                            strcat(buf,"/process.out");
                            execl(buf,"process.out", NULL);
                        }
                         int sendRunningTime;
                         message.mtype = processPid;
                         message.mRemainingTime = temp.runningTime;
                         sendRunningTime = msgget(2, 0666 | IPC_CREAT);//message queue to send message from the server
                         Down(sendRunningTime);
                    }
                    
                }
                else{
                    reading = false;
                    printf("stop reading\n");
                }
                up(sem2);
                prev = temp;
            }
            ProcessData firstInQueue;
            if(runningProcess.remainingTime == 0 && peek(readyQueue,&firstInQueue)){
                dequeue(readyQueue,&runningProcess);
                runningProcess.startTime = getClk();
                runningProcess.status = started;
                enqueue(PCB,runningProcess);
                //printf("my data pid = %d, remainingtime =%d\n",runningProcess.pid,runningProcess.arrivalTime);
            }
            if(runningProcess.remainingTime != 0){
                prevTime = currentTime;
                currentTime = getClk();
                if(prevTime!=currentTime){
                    //printf("remaining time = %d\n",runningProcess.remainingTime);
                    if(getClk()!=runningProcess.startTime){

                        runningProcess.remainingTime--;
                    }
                    message.mtype = runningProcess.pid;
                    message.mRemainingTime = runningProcess.remainingTime;
                    int sendRemainingTime = msgget(2, 0666 | IPC_CREAT);//message queue to send message from the server
                    Down(sendRemainingTime);
                    //kill(runningProcess.pid,SIGUSR1);
                    //printf("Current Time is %d\n", currentTime);
                    if(runningProcess.remainingTime == 0){
                        runningProcess.finishTime = getClk();
                        runningProcess.weightedTA = ((float)(runningProcess.finishTime - runningProcess.arrivalTime)/runningProcess.runningTime);
                        runningProcess.waitingTime = runningProcess.startTime - runningProcess.arrivalTime;
                        runningProcess.status = finished;
                        enqueue(PCB,runningProcess);
                    }
                }
            }

            if(reading == false && !peek(readyQueue,&firstInQueue) && runningProcess.remainingTime == 0){
                int totalRunTime = getClk();

                int msgq_idDown = msgget(2, 0666 | IPC_CREAT); //return the message queue
                msgctl(msgq_idDown,IPC_RMID,(struct msqid_ds*)0); // destroy the message queue
                ProcessData peek;
                while(dequeue(PCB,&peek)){
                    printf("id = %d, status = %d  finishtime = %d,  waiting time = %d  start time = %d\n",peek.id,peek.status,peek.finishTime,peek.waitingTime,peek.startTime);
                    
                    totalweightedTurnAround += peek.weightedTA;
                    totalWaitingTime += peek.waitingTime;
                    //usefulRunTime += peek.runningTime;
                    

                }
                printf("useful runtime = %d,  total runtime = %d\n",usefulRunTime,totalRunTime);
                CPU_utilization = ((float)(usefulRunTime)/totalRunTime);
                avgweightedTurnAround = totalweightedTurnAround/processNum;
                avgWaitingTime = (float)(totalWaitingTime/processNum);

                printf("CPU utilization = %.2f  avgWeightedTA = %.2f   avgWaitingTime %.2f\n", CPU_utilization,avgweightedTurnAround,avgWaitingTime);

                exit(0);
            }
            
        }
    }  
}  