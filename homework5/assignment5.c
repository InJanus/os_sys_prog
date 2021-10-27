#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// plan is to do a merge sort to call recursivley with multiple threads
int *list;

int* merge(){
    exit(0);
}

int* mergeSort(int* myleftlist, int* myrightlist){
    // merge two lists together into one but sorted
    if(myleftlist == myrightlist){ //not down to 1 value yet
        // int m_index = l_index+(r_index-l_index)/2; //find the middle index

        //going to try to do a little diffrent than stock
        //split the array into left and right lists that way there are no data overwriting whthin threads

        // mergeSort(mylist, l_index, m_index);
        // mergeSort(mylist, m_index+1, r_index);

        return merge();
    }
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
    int* leftlist, rightlist = (int*)malloc((length/2)*sizeof(int));
    for(int i = 0; i < length/2; i++){
        printf("%i : %i\n", i, *(list+i));
        leftlist[i] = *()
    }
    for(int i = length/2; i < length; i++){
        printf("%i : %i\n", i, *(list+i));
    }


    return 0;
}