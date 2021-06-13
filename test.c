#include <stdlib.h>
#include <stdio.h>
#include "Queue.h"
#include "CircularQueue.h"


int main(int argc, char* argv[])
{
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
	// CircularQueue* Q = (struct CircularQueue*) malloc(sizeof(struct CircularQueue*));
	// Q->front = 0;
	// Q->back = 0;
	// Q->max = 20;
	// Q->Arr = (int*) malloc(Q->max);
	// printf("%ld\n",sizeof(Q->Arr));
	// enqueueArr(Q,10);
	// printf("%d\n",Q->Arr[Q->back]);
	// enqueueArr(Q,20);
	// printf("%d\n",Q->Arr[Q->back]);
	// //printf("%ld\n",sizeof(Q->max));
	// int x;
	// dequeueArr(Q,&x);
	// printf("x=%d\n",x);


	Queue *Q = (Queue*) malloc(sizeof( Queue*));
	Q->backPtr = NULL;
	Q->frontPtr = NULL;

	for (int i = 0;i<processNum;i++){
		enqueue(Q,processArr[i]);
		printf("%d %d %d\n",Q->backPtr->item.arrivalTime,Q->backPtr->item.runningTime,Q->backPtr->item.priority);

	}

	for(int i=0; i<processNum + 2; i++){
		ProcessData x;
		if(dequeue(Q,&x)){
			printf("%d %d %d\n",x.arrivalTime,x.runningTime,x.priority);
		}
		else{
			printf("Empty\n");
		}
	}





	return 0;

}




