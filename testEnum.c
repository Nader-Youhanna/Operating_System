#include <stdlib.h>
#include <stdio.h>
//#include "Queue.h"
//#include "CircularQueue.h"
#include "headers.h"


enum State {
    didNotStart = 0,
    running = 1,
    readyState = 2,
};
int main(int argc, char* argv[])
{   
    enum State state = running;  
    char buffer[265];
    char buf[50];
    getcwd(buf,sizeof(buf));
    int x =5;
   
    //itoa(temp.runningTime,buffer,10); 
    strcat(buf,"/test.o");
    printf("%s\n",buf);
    itoa(x,buffer,10);
    execl(buf,"test.o",buf,buffer, NULL);

}