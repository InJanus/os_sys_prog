#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    char *myinput = ((char *)malloc(100 * sizeof(char)));
    char *filename = ((char*)malloc(100 * sizeof(char)));
    char *mode = ((char*)malloc(10*sizeof(char)));
    int argcount = 0;
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
                int count = 0;
                while(fgets(myinput, 100, stdin)){ //input pipe command; this is useful to see if the user used the pipe(|) command
                                                   //stdin is the pipe used from the console
                    printf("%s", myinput); //write to console
                    fputs(myinput, myfile); //write to file
                    fflush(myfile); //force write to file before close
                    count++; 
                }
                fclose(myfile); //close file
            }
        }
        argcount++;
    }
    return 0;
}