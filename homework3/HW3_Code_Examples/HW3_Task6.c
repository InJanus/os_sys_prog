//Brian Culberson
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int flag = 1;

void ignore_signal(int signum){
    if(flag == 1){//first time
        printf("\nI was sent signal number %d, but I choose to ignore it\n", signum);
        flag = 2;
    }else if(flag == 2){//second time
        printf("\nI was sent signal number %d, exiting!\n", signum);
        flag = 0;
    }else{ //shouldnt be the case but set to 0
        flag = 0;
    }
}

int main(){
    int childrennum = 1;
    int *pids = (int *)malloc(childrennum * sizeof(int)); //make sure to declare size of the array her 
    pid_t mychild;
    
    for(int i = 0; i < childrennum; i++){
        mychild = fork();
        if(mychild < 0){ //fork failed
            printf("Fork failed, returning");
        }else if(mychild == 0){ //child
            signal(SIGINT, ignore_signal);
            while(flag); //loop endlessly
            printf("child %i interupted\n", getpid());
            return 0; //return child
        }else{
            *(pids+i) = mychild;
        }
    }
    printf("Parent will now wait 5 sec\n");
    sleep(5); //wait 5 sec
    for(int i = 0; i < childrennum; i++){ //send signals
        printf("Sending interupt to child process %i\n", *(pids+i));
        kill(*(pids+i), SIGINT);
    }
    printf("Parent will now wait 5 sec\n");
    sleep(5); //wait 5 sec
    for(int i = 0; i < childrennum; i++){ //send signals
        printf("Sending interupt to child process %i\n", *(pids+i));
        kill(*(pids+i), SIGINT);
    }
}

/*
Informal Extra credit:
I get the same thing in my code and i found a good disscussion about it
https://stackoverflow.com/questions/11724141/how-long-does-it-take-for-a-non-blocked-signal-get-delivered
Mostly the OS decides on when the signal is delevered. you decide on what to send but
the OS decicdes when they get it.
*/