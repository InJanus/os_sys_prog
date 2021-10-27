#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>  // This is the header file that contains prototypes for
                      // pthreads structures and user functions

int c = 0;  // I'm declaring a global variable.  The memory for this variable
            // will be in the process DATA segment and will have global scope.
            // Every thread that accesses this variable will be accessing the
            // exact same value in the same memory spot in the data segment.
            // Naturally, this is potentially dangerous and could be a cause
            // of race conditions if there is more than one thread trying
            // to write or read and write at the same time.    

// The way threads work is that we "register" a function as a thread.  Once
// the function is "registered" and "launched" -- it will run LIKE a process 
// in that it will be scheduled like one, but it will share the same text, data,
// and heap segments with EVERY OTHER thread launched in the same process.
// Sometime threads are referred to as "lightweight processes".  This kind of makes
// sense.  It allows parallelism and multi-programming across multiple lines of 
// execution, but does not require full context switches in which the whole process
// memory map is spun down and the whole process put into a wait state whenever 
// a context switch occurs.  Threads can be scheduled, suspended, and restarted with 
// far less overhead – at least in theory.
//
// Now, if you look at this code, you may note that I'm defining it with a * 
// in front of the name.  This essentially means that the token fnC is actually
// a POINTER to the function while *fnc() is actually a CALL to the function.
// Pointers to functions are actually kind of cool and you may want to review 
// them if you are not familiar with them.  We'll use it here so we can tell
// one function (the pthreads routine that registers and launches threads) that
// the thread should RUN the code that lives at the end of a function pointer.
//
// The function itself is kind of simple.  It just counts from zero to nine and
// for each step it increments the global variable "c" and then prints the value
// to the screen.  Now, if I were to make two calls to *fnC() as NORMAL functions,
// not threads, this would print the following:
//
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
//
// Of course, a naive programmer might think that if I were to run fnC() as two
// threads, I'd get the same result -- but potentially "faster" because of 
// multiple cores.  Of course, you already know this won't happen :)

void *fnC()
{
    int i;
    for(i=0;i<10;i++)
    {   c++;
        printf(" %d", c); 
    }      
   pthread_exit(0);
}

int main()
{
    // These will be return values from the routine that launches threads 
       int rt1, rt2;
    
    // These are variables of pthread_t, they hold bookkeeping values for threads.
    // In this case we’ll launch two threads, so we need two of these.
    pthread_t t1, t2; 

    // We’re going to run a bunch of thread experiments and examine the outputs.
    // This variable will keep track of which trial we’re on.

    int trial_count = 0;
    
    // For every trial, lets zero out the counter and run the count routine “twice”
    // as threads that can be scheduled onto independent cores instead of running
    // in sequence.

    for (trial_count = 0; trial_count < 1000; trial_count++)
        {  // Zero out the global counter
           c = 0;

           // Create two thread with the routine pthread_create().  You can use
           // reference materials to get definitions of what the various parameters 
           // mean or just refer to the appropriate part of chapter 4 in the book. 
           // The short version is this:
           //
           // pthread_create(pointer_to_a_thread_id_block,
           //                pointer_to_attribute_block,
           //                pointer_to_a_function_to_run_in_thread,
           //                pointer_to_parameters_for_thread_function)
           //
           //                In this case, we're using "default" thread attributes, so there's a NULL
           //                there in the code below, and we're not passing parameters to the threads, 
           //                so also appropriate NULLs below.  We'll complicate this later, trust me.
           //

           if((rt1=pthread_create( &t1, NULL, fnC, NULL)))
              printf("Thread creation failed: %d\n", rt1);
              
           if((rt2=pthread_create( &t2, NULL, fnC, NULL)))
              printf("Thread creation failed: %d\n", rt2);

          // Wait for both threads to finish.  The main process thread will block
          // until the threads that were launched terminate.  Once they both finish,
          // then the “main thread” unblocks and continues.

          pthread_join(t1, NULL);
          pthread_join(t2, NULL);
          printf("\n");
        }

    return 0;
}