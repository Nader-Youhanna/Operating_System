#include <stdlib.h>
#include <stdio.h>

#include "CircularQueue.h"
#include "process_data.h"



int main(int argc, char* argv[])
{
    CircularQueue* Q = (CircularQueue*) malloc(sizeof(CircularQueue));
    Q->back = 0;
    Q->front = 0;
    Q->max = 5;
    Q->Arr = (ProcessData*) malloc((Q->max)* sizeof(ProcessData));

    for(int i =0;i<5;i++){
        ProcessData temp1;
        temp1.id =i;

        enqueueArr(Q,temp1);
    }

    for(int i =0;i<7;i++){
        ProcessData temp1;
        if(dequeueArr(Q,&temp1)){
            printf("%d\n",temp1.id);

        }else{
            printf("empty\n");
        }
    }

    ProcessData temp1;

    if (isEmptyArr(Q))
    {
        printf("Success\n");
    }

    temp1.id = 1;
    enqueueArr(Q,temp1);
    temp1.id = 2;
    enqueueArr(Q,temp1);

    ProcessData temp2;
    if (dequeueArr(Q,&temp2))
    {
        printf("Success\n");

        if (temp2.id == 1)
        {
            printf("Success\n");
        }
    }







	return 0;

}




