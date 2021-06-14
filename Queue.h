#ifndef Queue_h
#define Queue_h
#include<stdlib.h>
#include<stdio.h>
#include "Node.h"
#include <stdbool.h>
#include "process_data.h"



typedef struct Queue
{
	struct Node* backPtr;
	struct Node* frontPtr;
}Queue;



bool isEmpty(struct Queue* Q) 
{
	return (Q->frontPtr == NULL);	
}


//int will be replaced by struct containing info about process
//bool could be converted to void
bool enqueue(struct Queue* Q, const ProcessData newEntry)	
{

	struct Node* newNodePtr = (struct Node*) malloc(sizeof(struct Node));
	newNodePtr->next = NULL;
	
	//NodeConstructor(newNodePtr);
	
	newNodePtr->item = newEntry;
	
	if (isEmpty(Q)) 
	{ 
		
		Q->frontPtr = newNodePtr; 
	}
	else
	{	
		
		Q->backPtr->next = newNodePtr; 
	}
	
	
	Q->backPtr = newNodePtr; 
	
	return true;
} 

bool peek(struct Queue* Q,ProcessData* frntEntry) 
{
	//printf("%p\n",Q);
	//printf("%d\n",Q->frontPtr->item);
	if (isEmpty(Q))
		return false;
	else
		*frntEntry = (ProcessData) Q->frontPtr->item;
		//printf("%d\n",Q->frontPtr->item);
	return true;

}

//int will be replaced by struct containing info about process
bool dequeue(struct Queue* Q, ProcessData* frntEntry)
{
	if (isEmpty(Q))
		return false;

	struct Node* nodeToDeletePtr = (struct Node*) malloc(sizeof(struct Node));
	nodeToDeletePtr = Q->frontPtr;
	*frntEntry = (ProcessData) Q->frontPtr->item;
	Q->frontPtr = Q->frontPtr->next;
	
	// Queue is not empty; remove front
	if (nodeToDeletePtr == Q->backPtr)	 // Special case: last node in the queue
		Q->backPtr = NULL;

	return true;
}



/*void enqueue(struct Queue* Q,const int item)
{
	struct Node* N;
	N->item = item;
	printf("%d\n",N->item);
	Q->backPtr = N;
	Q->backPtr->item = N->item;
	printf("%d\n",N->item);
	
}
*/

#endif










