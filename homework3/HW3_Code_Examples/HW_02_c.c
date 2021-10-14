// Program HW_02_c.c
#include <stdlib.h>
#include <stdio.h>

// extern char **environ; // look into what “extern” means when applied to 
                       // a C global variable :)
//dont need this anymore

int main(int argc, char **argv, char **envp){ // added envp
  while (*envp != NULL){ //you can change this to envp
    printf("%s\n", *envp); //print envp
    envp++; //increment envp
  }
  printf("\n");
}