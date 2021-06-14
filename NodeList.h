#ifndef NodeList_h
#define NodeList_h



struct NodeList
{
	ProcessData item;  // A Process item
	
	struct NodeList* next; // Pointer to next node

	int priority; // integer representing the priority of the processes (the lower the integer the higher the priority)
}; 





#endif