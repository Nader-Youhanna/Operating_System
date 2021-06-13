#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "headers.h"
#include <unistd.h>
#include <limits.h>

#include "Queue.h"
#define SHKEY 300

void clearResources(int);
int shmid;


/* arg for semctl system calls. */
union Semun
{
    int val;              
    struct semid_ds *buf;  
    ushort *array;         
    struct seminfo *__buf; 
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

int main(int argc, char* argv[])
{

    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE* f;
    char* path = argv[1];
    char* mode = "r";
    char buff[255];
    f = fopen(path, mode);

    int processNum = 0;
    for (int i = 0; i < 4; i++) fscanf(f, "%s", buff);
    while (fscanf(f, "%s", buff) != EOF)
    {

        for (int i = 0; i < 3; i++) 
        {
        	fscanf(f, "%s", buff);
        	//printf("%d ",atoi(buff));
        }
        //printf("\n");
        processNum++;

    }
   // printf("Finished Reading %d\n",processNum);
    struct ProcessData processArr[processNum];
    int totalRunningTime = 0;
    f = fopen(path, mode);
    for (int i = 0; i < 4; i++) fscanf(f, "%s", buff);
    for (int i = 0; i < processNum; i++)
    {
        fscanf(f, "%s", buff);
        processArr[i].id = atoi(buff);
        fscanf(f, "%s", buff);
        processArr[i].arrivalTime = atoi(buff);

        fscanf(f, "%s", buff);
        processArr[i].runningTime = atoi(buff);
        totalRunningTime += processArr[i].runningTime;
        fscanf(f, "%s", buff);
        processArr[i].priority = atoi(buff);
    }
    printf("%d\n",totalRunningTime);
    
    /*for (int i = 0; i < processNum ; i ++)
    {
    	printf("%d %d %d\n",processArr[i].arrivalTime,processArr[i].runningTime,processArr[i].priority);
     
    }*/

    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    int algorithm = atoi(argv[3]);
    int quantum;
    //int shmid;
    if (algorithm == 5) //ROundRobin
    {
        
        quantum = atoi(argv[5]);

    }
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
    *shmaddrAlgorithm = algorithm;
    //printf("%d\n",quantum);
    // 3. Initiate and create the scheduler and clock processes.
    /*char buf[50];
    getcwd(buf,sizeof(buf)); writes home/maestro619/Desktop/Project in buf
    printf("%s\n",buf);*/
    
    int schedulerPid = fork();
    if (schedulerPid == 0)
    {
        printf("Scheduler\n");
        execl("/home/maestro619/Desktop/Project/scheduler.out","scheduler.out",NULL);

    }
    else
    {
    	//return 0;
        int clk = fork();
        if (clk == 0)
        {
            execl("/home/maestro619/Desktop/Project/clk.out","clk.out", NULL);
        }
        else
        {
            

            // 4. Use this function after creating the clock process to initialize clock.
            initClk();
            // To get time use this function. 
            int currentTime = getClk();
            
            printf("Current Time is %d\n", currentTime);
            
            // TODO Generation Main Loop

            
            // 5. Create a data structure for processes and provide it with its parameters.
            if (1)
            {   
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

            	Queue* Q = (Queue*) malloc(sizeof(Queue*));
                for(int i = 0; i<processNum; i++){
                    enqueue(Q,processArr[i]);
                }

                int prevTime;
                while(processNum>0){
                    //printf("processnum = %d\n",processNum);
                // 6. Send the information to the scheduler at the appropriate time.
                    ProcessData temp;
                    if(peek(Q,&temp)){
                        if(temp.arrivalTime == currentTime){
                            if(dequeue(Q,&temp))
                            {
                                *shmaddr = temp;
                                processNum--;
                            }

                        }
                    }
                    
                    up(sem1);
                    prevTime = currentTime;
                    currentTime = getClk();
                    if(prevTime!=currentTime){

                        printf("time = %d\n",currentTime);
                    }
                    
                    down(sem2);
                }
                ProcessData temp;
                temp.id = -1;
                *shmaddr = temp;
                printf("%d\n",shmaddr->id);
                up(sem1);
                down(sem2);
                // 7. Clear clock resources
                
                shmctl(shmid, IPC_RMID,(struct shmid_ds *)0);
                shmctl(shmidAlgo, IPC_RMID,(struct shmid_ds *)0);
                semctl(sem1,0,IPC_RMID);
                semctl(sem2,0,IPC_RMID);
            }
            printf("finished\n");
            int* stat_loc;
            wait(stat_loc);
	        destroyClk(true);
        }
    }
}
void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    //printf("\nDeleting Shared Memory and Semaphore Set\n");
    shmctl(shmid, IPC_RMID, (struct shmid_ds*)0);
    exit(-1);
}