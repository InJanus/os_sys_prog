// Program HW_02_a.c 
#include <stdlib.h> 
#include <stdio.h>

int main(int argc, char **argv){
  int arg_count = 0; //still need to use this for the loop
  while(arg_count < argc){ //this wile loop replaces the for loop.
    printf("%s\n", *(argv + arg_count)); //changing the array logic to pointer arithmitic.
    arg_count = arg_count + 1; //increment counter so while loop works
  }
}