#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>  // This is the header file that contains prototypes for
                      // pthreads structures and user functions

double c[100];  // I'm declaring a global variable.  The memory for this variable
                // will be in the process DATA segment and will have global scope.
                // I'll fill this array with data later, and each of the threads
                // I'll write will eventually use it.  In this code, NONE parallel
                // threads will ever try to change the contents of the array, so 
                // we won't get any data corruption problems, at least not of THIS
                // array.
             
// We CAN pass parameters to a thread, but we have to pass ALL of the parameters as
// via a single pointer.  If we wanted to pass a single parameter, we could just pass
// a pointer to something of the right type and be done with it.  If we want to send
// "multiple" parameters, we must define a compound data type that has all of them in 
// it, send a pointer to that compound type, then have the thread "take it apart" 
// inside.  Let's do that right now....

// First, I'll define a struct that contains fields for the things I want to send into
// the thread.  In this case, I'll want to send two integers for "start" and "end"
// indices into the array (c[]) that we'll be processing.  For convenience, I gave 
// names to the argument type AND a name for the pointer to that type.  This was not
// necessary, but I like doing it because that's how I roll.  You may see other code
// that isn't so wordy like mine...
// 
// Also, I'm playing another trick...  I'll RETURN a value via the same struct.  When
// I pass the pointed to the struct INTO the thread, that pointer can be used to pull 
// things in AND put them out :)

typedef struct arg 
          { int start_index;  // Input parameter
            int end_index;    // Input parameter
            double sum;       // Space for return value
          }  arg_type;

typedef arg_type *arg_ptr;

// Now, let's see how we get those arguments into the threads and how the threads can
// interpret their arguments.... consider the following thread function code: 
                     
void *sum_array_elements(void *input)
// Look at that... we'll send in ALL the parameters as a single pointer to struct.
// Sure, we'll send it in as a pointer to a void type, but we'll cast it to the right
// type INSIDE the function.  So let's do that in the context of code that tries to 
// get something done...

{   int i;      // This is a local variable. EVERY thread will get its own local
                // version of this.  So no chance of causing a data integrity issue
                // by having threads modify their own local copies of something....
                
    int start_index;  // More local variables.  No danger here.  These are local to
    int end_index;    // the thread :)
    double sum;
    
    // Here I'm copying the parameters FROM the parameter block into local variables
    // for use.  Granted, I didn't need to do this... I could have just used the 
    // pointer arithmetic expressions directly, but for sake of clarity (if not 
    // efficiency) I'm doing it here.
    
    start_index = ((arg_ptr)input)->start_index;
    end_index   = ((arg_ptr)input)->end_index;
    
    // Now, let's do a sum of all the elements of array c[] between the start and
    // end indices.  Note we're doing NO error checking on bounds here.  This is 
    // both dangerous and stupid, but that's an issue for a later module :)
    
    sum = 0.0;
    for(i=start_index; i <= end_index; i++)
       sum += c[i];
     
    // Now, another issue.  How do we return values from threads?  Lots of options 
    // here.  We'll go with a simple one of just.... stuffing into the same struct
    // I used to get stuff INTO the thread...
    
    ((arg_ptr)input)->sum = sum;
    
    // Ok, we're out of here!     
   pthread_exit(0);
}

int main()
{
    // A counter variable we'll use to fill the array c[] with stuff.  Local to 
    // main()
    int i;

    // We'll put the average of all of the values in c[] in here eventually....
    double average;
    
    // These will be return values from the routine that launches threads 
       int rt1, rt2;
    
    // These are variables of pthread_t, they hold bookkeeping values for threads.
    // In this case we’ll launch two threads, so we need two of these.
    pthread_t t1, t2; 
    
    // Let's make some structs to hold input parameters to the threads...
    arg_type args_1, args_2;

    // Let's go ahead and put some random data into the global array c[]
    time_t t;
    srand((unsigned)time(&t));        
    for (i=0; i < 100; i++)
        c[i] = (double)((rand()%1000)/1000.0);
        
    // We’re going to run a bunch of thread experiments and examine the outputs.
    // This variable will keep track of which trial we’re on.
    int trial_count = 0;
    
    // For every trial, lets zero out the counter and run the count routine “twice”
    // as threads that can be scheduled onto independent cores instead of running
    // in sequence.

    for (trial_count = 0; trial_count < 10; trial_count++)
        {  
           // Let's set up the args.  I want thread one to work on the first
           // half of the array c[] and the thread two to work on the second half
           // of array c[].
           args_1.start_index = 0;
           args_1.end_index   = 49;
           args_2.start_index = 50;
           args_2.end_index   = 99;

           // Create two thread with the routine pthread_create().  You can use
           // reference materials to get definitions of what the various parameters 
           // mean.

           if((rt1=pthread_create( &t1, NULL, sum_array_elements, (void *)&args_1)))                  
              printf("Thread creation failed: %d\n", rt1);
               
           if((rt2=pthread_create( &t2, NULL, sum_array_elements, (void *)&args_2)))
              printf("Thread creation failed: %d\n", rt2);

          // Wait for both threads to finish.  The main process thread will block
          // until the threads that were launched terminate.  Once they both finish,
          // then the “main thread” unblocks and continues.

          pthread_join(t1, NULL);
          pthread_join(t2, NULL);
          
          // Now that the threads are finished, let's take the two partial sums and 
          // use them to compute the average of ALL of the values in the array 
          // c[].
          
          average = (args_1.sum + args_2.sum)/100.0;
          printf("The average of all the values in array c[] is %lf\n", average);
          printf("\n");
        }

    return 0;
}