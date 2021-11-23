#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// int *list;
int* output;
typedef struct arg 
          { int* input;
            int length;
            int* output;
          }  arg_type;

typedef arg_type *arg_ptr;

void *sort(void* input){
    int length = ((arg_ptr)input)->length;
    int* list = ((arg_ptr)input)->input;
    //printf("hello from thread!");
    //bubble sort the list
    output = (int*)malloc(length*sizeof(int));
    int* temp;
    for(int k = 0; k < length; k++){
        temp = list;
        while(*temp == 0){
            temp++;
        }
        for(int i = 0; i < length; i++){
            if(*temp > list[i]){
                if (list[i] != 0){
                    temp = (list+i);
                }
            }
        }
        output[k] = *temp;
        *temp = 0;
    }
    // return output;
    ((arg_ptr)input)->output = output;
    pthread_exit(0);
}

int main(){
    pthread_t t1, t2;
    arg_type args_1, args_2;
    int rt1, rt2;
    // get list of integers from file
    char *myinput = ((char *)malloc(100 * sizeof(char)));
    char *filename = ((char*)malloc(100 * sizeof(char)));
    char *mode = ((char*)malloc(sizeof(char)));
    int* list = (int*)malloc(10000*sizeof(int));
    int count = 0;
    filename = "output.txt";
    mode = "r";
    FILE *myfile = fopen(filename, mode);
    while(fgets(myinput, 100, myfile)){
        list[count] = atoi(myinput);
        // printf("%i\n", list[count]);
        count++;
    }
    int length = count;
    // printf("%i\n", length);

    int* leftlist = (int*)malloc((length/2)*sizeof(int));
    int* rightlist = (int*)malloc((length/2)*sizeof(int));

    //split the list in half
    for(int i = 0; i < length/2; i++){
        leftlist[i] = *(list+i);
        // printf("%i\n",leftlist[i]);
    }
    // printf("rightlist");
    for(int i = 0; i < length/2; i++){
        rightlist[i] = *(list+i+(length/2));
        // printf("%i\n",rightlist[i]);
    }
    // set params to the threads
    args_1.input = leftlist;
    args_1.length = length/2;
    args_2.input = rightlist;
    args_2.length = length/2;

    //create threads
    if((rt1=pthread_create( &t1, NULL, sort, (void *)&args_1))){
        printf("Thread creation failed: %d\n", rt1);
    }
              
    if((rt2=pthread_create( &t2, NULL, sort, (void *)&args_2))){
        printf("Thread creation failed: %d\n", rt2);
    }

    pthread_join(t1, NULL); //join threads
    pthread_join(t2, NULL);
    
    int* sortedleftlist = (int*)malloc(length/2*sizeof(int));
    int* sortedrightlist = (int*)malloc(length/2*sizeof(int));
    sortedleftlist = args_1.output; //geting the two sorted lists from the threads
    sortedrightlist = args_2.output;

    //merge sort the two lists
    int* sortedlist = (int*)malloc(length*sizeof(int));
    int leftcount = 0, rightcount = 0;
    for(int i = 0; i < length; i++){
        //printf("left:%i | right:%i\n", sortedleftlist[leftcount], sortedrightlist[rightcount]);
        if(sortedleftlist[leftcount] == 0){ //checks to see if it is 0
            //take right number
            sortedlist[i] = sortedrightlist[rightcount];
            rightcount++;
        }else if(sortedrightlist[rightcount] == 0){ //checks to see if it is 0
            sortedlist[i] = sortedleftlist[leftcount];
            leftcount++;
        }else if(sortedleftlist[leftcount] < sortedrightlist[rightcount]){ //checks to see if the left side is smaller than the right
            sortedlist[i] = sortedleftlist[leftcount];
            leftcount++;
        }else if(sortedleftlist[leftcount] >= sortedrightlist[rightcount]){ //checks to see if the left is greater than the right
            sortedlist[i] = sortedrightlist[rightcount];
            rightcount++;
        }
    }

    //output to screen with index
    for(int i = 0; i < length; i++){
        printf("%i:%i\n",i,sortedlist[i]);
    }
    return 0;
}