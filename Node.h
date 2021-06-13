#ifndef Node_h
#define Node_h


#include "process_data.h"
struct Node
{
	ProcessData item; // A data item
	//int will be replaced by struct containing info about process
	struct Node* next; // Pointer to next node
}; 


/*void NodeConstructor(struct Node* N) 
{
	N = (struct Node*) malloc(sizeof(struct Node));
	N->next = NULL;
}*/ 

void setItem(struct Node* N, const ProcessData item)
{
	N->item = item;
}

void setNext(struct Node* N, struct Node* Next)
{
	N->next = Next;
}

#endif
