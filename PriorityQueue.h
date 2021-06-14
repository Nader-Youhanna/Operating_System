#ifndef PriorityQueue_h
#define PriorityQueue_h
#include<stdlib.h>
#include<stdio.h>
#include "NodeList.h"
#include <stdbool.h>
#include "process_data.h"

typedef struct PriorityQueue
{
	struct NodeList* backPtr;
	struct NodeList* frontPtr;
}PriorityQueue;


bool isEmptyList(struct PriorityQueue* Q) 
{
	return (Q->frontPtr == NULL);	
}


//int will be replaced by struct containing info about process
//bool could be converted to void
bool enqueueList(struct PriorityQueue* Q, const ProcessData newEntry, const int p)	
{

	struct NodeList* newNodePtr = (struct NodeList*) malloc(sizeof(struct NodeList));
	newNodePtr->next = NULL;
	newNodePtr->priority=p;
	newNodePtr->item = newEntry;
	
	if (isEmptyList(Q)) //the case that the list is empty
	{ 
		Q->frontPtr = newNodePtr;
                Q->backPtr = newNodePtr; 
               
	}
	else
	{	if(Q->frontPtr->priority > p) // if the new node has a higher priority then the front of the list we replace the front
		{
		    newNodePtr->next=Q->frontPtr;
		    Q->frontPtr = newNodePtr;
		}
                else 
		{
		struct NodeList* temp = Q->frontPtr;// traversing the list to find the right place to put the new node

		while (temp->next != NULL && temp->next->priority < p)
		 {
		    temp = temp->next;
		 }
	 
		newNodePtr->next = temp->next;
		temp->next = newNodePtr;
		
		//if the new node is at the end of the queue
		if(newNodePtr->next==NULL)
		Q->backPtr = newNodePtr;
		
		
		} 
	}
	//Q->backPtr = newNodePtr; 
	return true;
} 
bool peekList(struct PriorityQueue* Q,ProcessData* frntEntry) 
{
	if (isEmptyList(Q))
		return false;
	else
		*frntEntry = Q->frontPtr->item;
	return true;

}

//int will be replaced by struct containing info about process
bool dequeueList(struct PriorityQueue* Q, ProcessData* frntEntry)
{
	if (isEmptyList(Q))
		return false;

	struct NodeList* nodeToDeletePtr = (struct NodeList*) malloc(sizeof(struct NodeList));
	nodeToDeletePtr = Q->frontPtr;
	*frntEntry = Q->frontPtr->item;
	Q->frontPtr = Q->frontPtr->next;
	
	// Queue is not empty; remove front
	if (nodeToDeletePtr == Q->backPtr)	 // Special case: last node in the queue
		Q->backPtr = NULL;

	return true;
}





#endif