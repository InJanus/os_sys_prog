#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MSGSIZE 100

int main(int argc, char **argv){
    char *myinput = ((char *)malloc(100 * sizeof(char)));
    char *filename = ((char*)malloc(100 * sizeof(char)));
    char *mode = ((char*)malloc(10*sizeof(char)));
    char inbuf[MSGSIZE]; 
    int argcount = 0;
    int status;
    mode = "w";
    filename = "";
    while(*(argv+argcount)){ //append argument
        if(strcmp(*(argv+argcount),"-a") == 0 || strcmp(*(argv+argcount),"--append") == 0){
            mode = "a";
        }else if(strcmp(*(argv+argcount),"--version") == 0){ //--version argument
            printf("This is a copy of the tee command\nWritten by Brian Culberson\n");
            return 0;
        }else{
            if(argcount != 0){
                filename = *(argv+argcount); //get file name
                FILE *myfile = fopen(filename, mode); //open file
                pid_t  pid;
                int p[2]; //needed for pipe
                if (pipe(p) < 0) exit(1); //set up pipe
                if ((pid = fork()) < 0) { //fork into two processes
                    printf("forking child process failed\n");
                    return 0;
                }
                if(pid == 0){
                    //child
                    close(p[1]); //read only
                    while (read(p[0], inbuf, MSGSIZE)){ //read from pipe
                        printf("%s", inbuf); //write to console
                        fputs(inbuf, myfile); //write to file
                        fflush(myfile); //force write to file before close
                    }
                }else{
                    //parent
                    close(p[0]); //write only
                    while(fgets(myinput, 100, stdin)){
                        write(p[1], myinput, MSGSIZE); //send buffer through pipe
                    }
                    close(p[1]); //close pipe
                    wait(pid, &status, 0); //wait for child
                }
                fclose(myfile); //close file
            }else{
                pid_t  pid;
                int p[2]; //needed for pipe
                if (pipe(p) < 0) exit(1); //set up pipe
                if ((pid = fork()) < 0) { //fork into two processes
                    printf("forking child process failed\n");
                    return 0;
                }
                if(pid == 0){
                    //child
                    close(p[1]); //read only
                    while (read(p[0], inbuf, MSGSIZE)){ //read from pipe
                        printf("%s", inbuf); //write to console
                    }
                }else{
                    //parent
                    close(p[0]); //write only
                    while(fgets(myinput, 100, stdin)){
                        write(p[1], myinput, MSGSIZE); //send buffer through pipe
                    }
                    close(p[1]); //close pipe
                    wait(pid, &status, 0); //wait for child
                }
            }
        }
        argcount++;
    }
    return 0;
}