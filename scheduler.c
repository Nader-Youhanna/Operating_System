#include "headers.h"
#include "Queue.h"


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
    ProcessData prev;
    ProcessData temp;
    bool read = true;

    if(algorithm==1){
        while(1){

            if(read){
                down(sem1);
                prev = temp;
                temp = *shmaddr;
                
                if(temp.id != -1){
                    if(temp.id!=prev.id){
                        printf("%d\n",temp.id);
                        if(algorithm == 1){

                            printf("%d %d %d\n",temp.arrivalTime,temp.runningTime,temp.priority);
                        }

                    }
                    
                }
                else{
                    read = false;
                    printf("stop reading\n");
                }
                up(sem2);

            }
            else{
                //printf("done reading\n");
            }
            //printf("read = %d\n",read);



            //exit(0);
        }
    }
    
    


    //TODO: upon termination release the clock resources.
    destroyClk(true);
}
