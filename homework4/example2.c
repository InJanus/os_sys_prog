// Pipes Example #2

#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h> 
#define MSGSIZE 32 

char* msg1 = "hello "; 
char* msg2 = "world"; 

int main() 
{ 
    pid_t child;
    char inbuf[MSGSIZE]; 
    int p[2], i; 
    int status;

    // In the next line, I'll establish a pipe by asking the kernel to make it so
    // and then put the read side and write side descriptors into the array I provide
    // This is JUST like we did before...

    if (pipe(p) < 0) exit(1); 
          
    // Now I'm going to make a child.  When the child is made, it will have its OWN
    // version of array p and that array will be filled with the same descriptors that
    // the originating parent had.  Parent and child both know about the same kernel 
    // mediated communication path :)

    child = fork();

    // At this point, there are TWO processes that are running this code.  Each of them
    // will pick up right here.  One of them will know it's the child and run child code.
    // The other will know it's the parent and run parent code

    // Here's the child code.  I want the child to read a bunch of message
    // from the pipe and print them out.  Note this code is not making any 
    // assumptions about how many messages there will be.  Presumably it's 
    // possible for a "read" to fail because the pipeline closed down and 
    // this child code is relying on that to know when to stop reading from
    // it

    if (child == 0)
       { close(p[1]);   // Close down the "write" side of the pipe because the child is not writing
  
         // Read from the pipe while there are things to read...
         while (read(p[0], inbuf, MSGSIZE))
            printf("%s", inbuf);
         printf("\n");
         // Well... the child process is done, so exit
         exit(0);
       }

    // Here's the parent code.  The parent wants to send data to the child, so it does
    else
       { close(p[0]);   // Close the read side of the pipe, because the parent is not reading
         
         // send ten messages into the pipe.  Note some funny business here.  I'm 
         // writing five, waiting five seconds, then writing five more.  Curious... 
         // we'll explore this later in the lab assignment writeup.

         for (i=0; i < 5; i++) write(p[1], msg1, MSGSIZE); 
         sleep(5);
         for (i=0; i < 5; i++) write(p[1], msg1, MSGSIZE); 
       
         // CLOSE the write side of the pipe.  Why is this important?  Well, if the pipe
         // is never closed here, how with the child ever know the messages are not coming
         // anymore.

         close(p[1]);
         
         // Now the parent waits around for its child to do its work, and only after the
         // child is done will it terminate.
         waitpid(child, &status, 0);
       }
}