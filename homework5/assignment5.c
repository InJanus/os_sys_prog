#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// plan is to do a merge sort to call recursivley with multiple threads
int *list;

int* merge(){
    exit(0);
}

int* runMergeSort(int* myleftlist, int* myrightlist, int leftlen, int rightlen){

}

int* mergeSort(int *mylist, int length){
    int* leftlist = (int*)malloc((length/2)*sizeof(int));
    int* rightlist = (int*)malloc((length/2)*sizeof(int));
    int leftlen = length/2, rightlen = length/2;
    for(int i = 0; i < length/2; i++){
        printf("%i : %i\n", i, *(list+i));
        leftlist[i] = *(list+i);
    }
    for(int i = length/2; i < length; i++){
        printf("%i : %i\n", i, *(list+i));
        rightlist[i] = *(list+i);
    }
    printf("%i,%i", leftlen, rightlen);
    
    runMergeSort(leftlist, rightlist, leftlen, rightlen);
}

int main(){
    // get list of integers from file
    char *myinput = ((char *)malloc(100 * sizeof(char)));
    char *filename = ((char*)malloc(100 * sizeof(char)));
    char *mode = ((char*)malloc(sizeof(char)));
    list = (int*)malloc(20*sizeof(int));
    int count = 0;
    filename = "output.txt";
    mode = "r";
    FILE *myfile = fopen(filename, mode);
    while(fgets(myinput, 100, myfile)){
        list[count] = atoi(myinput);
        count++;
    }
    int length = count;
    printf("%i\n", length);
    // checking
    // while(*list){
    //     printf("%i\n", *list);
    //     list++;
    // }

    // split the array in half and call the first few threads

    mergeSort(list, length);
    return 0;
}