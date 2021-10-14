#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// When we eventually start sending and receiving messages through the pipeline, 
// we'll do it in chunks of a certain size of characters.  For this code, we're 
// going to say that messages (character strings) will go into the pipe in chunks
// of 12 characters.  Every time we read or write, we'll get chunks of up to
// 12 characters in length.  This constant we're about to set will be used
// when we make pipe calls to set the chunk size.  You can set it to any reasonable
// value.
 
#define MSGSIZE 12

// Here's some messages that we'll later send through the pipe.
// Notice the messages are declared as strings.  Why?  Because
// pipes read and write character string data.

char* msg1 = "hello ";
char* msg2 = "world ";

int main()
{   char inbuf[MSGSIZE];  // This is just a buffer that our process will use to 
                          // receive data from the pipe.  The pipe read call will
                          // take characters out of the pipe in chunks of a specified
                          // size.  Note that we're making the buffer the same 
                          // size as our defined message size.

    int p[2];             // Unix/Posix pipes represent the "pipe" as a two character integer array.
                          // Each slot of the array contains an "integer" that is a designator for 
                          // a pseudo-text file.  The first slot is the ID of the "side you read from"
                          // and the second slot is the ID of the "side you write to".  Note that
                          // both sides refer to the same pipe, they just refer to the two sides of it.
                          // Also note that pipes are STRICTLY one-way communication.  You WRITE to one
                          // side and you READ from the other.  Period... this will become more relevant
                          // later.

    int i;                // Just a generic counting variable

    // Here's our first actual pipe related code.  The pipe() routine establishes a pipe by telling the 
    // kernel to make one and putting the identifiers of the read and write sides into the integer array
    // we provide, in this case, the array p.  the pipe() call returns a negative value if the attempt
    // to make the pipe fails.  If that happens, we exit the program.

    if (pipe(p) < 0) exit(1);     // If I can’t make a pipe, exit the program  

    // Here I’m writing two messages to the pipe.  They will go into the “virtual
    // file” established by the pipe.  The kernel will hold these until they are 
    // read.  They will come out in queue order when read (first in, first out).

    write(p[1], msg1, MSGSIZE);   // Put message one into the WRITE side of the pipe
    write(p[1], msg2, MSGSIZE);   // Put message two into the WRITE side of the pipe

    // Ok, so now I’m going to read two messages from the same pipe.  This time 
    // I’m referring to the “output” side of the pipe (p[0]).  Then I’ll print them
    // as indicated

    for (i = 0; i < 2; i++) {
        /* read pipe */
        read(p[0], inbuf, MSGSIZE);
        printf("%s", inbuf);
    }

    printf("\n");
    return 0;
}