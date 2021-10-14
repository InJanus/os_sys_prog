//Brian Culberson
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int children_count = 10;
    pid_t mychildren;

    //get children count
    printf("How many children should I create: ");
    scanf("%i", &children_count);
    printf("Creating %i children\n", children_count);

    for(int i = 0; i < children_count; i++){
        mychildren = fork(); //fork all the children
        if(mychildren < 0){ //error checking
            printf("The fork() didn't work! Terminate\n"); 
            return 0;
        }else if(mychildren == 0){ //if child go here
            printf("Hello from child ------- %i\n", getpid());
            return 0; //exit child so it does not keep looping
        }
    }
    wait(NULL); //wait for children to finish
    printf("Hello from parent ====== %i\n", getpid()); //parent process finishes
    return 0;
}