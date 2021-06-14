#ifndef CircularQueue_h
#define CircularQueue_h
#include<stdlib.h>
#include<stdio.h>
#include "Node.h"
#include <stdbool.h>




typedef struct CircularQueue
{
	ProcessData* Arr;
	int front, back;
	int max ;
}CircularQueue;

bool isEmptyArr(struct CircularQueue* Q)
{
	return(Q->front == Q->back);
}

bool isFullArr(struct CircularQueue* Q)
{
	return(Q->front == (Q->back + 1) % (Q->max + 1));

}


bool enqueueArr(struct CircularQueue* Q,const ProcessData item)
{   //printf("inside enque\n");
    //printf("item = %d,  first element in arr = %d\n",item,Q->Arr[Q->front]);
	if (isFullArr(Q))
		return false;

	else
	{
        printf("adding element\n");
		Q->back = (Q->back + 1) % (Q->max + 1);
        printf("back = %d \n",Q->back);
		Q->Arr[Q->back] = item;
		return true;
	}
}

bool dequeueArr(struct CircularQueue* Q,ProcessData* item)
	{
		if (isEmptyArr(Q))
		{
			return false;
		}
		else
		{
			Q->front = (Q->front + 1) % (Q->max + 1);
			*item = Q->Arr[Q->front];
			return true;
		}

	}





#endif
