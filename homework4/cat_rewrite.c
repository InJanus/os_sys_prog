#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h> 
#define MSGSIZE 6

int main(int argc, char **argv) 
{   
    char c;
    FILE *fp;
    
    if (argc > 1)
       { fp = fopen(argv[1], "r");
         if (fp == NULL) 
           { fprintf(stderr, "ERROR - FILE %s NOT FOUND\n", argv[1]);
             exit(1);
           } 
       }
    else fp = stdin;
        
    while (!feof(fp))
       { c = getc(fp);
         if (!feof(fp)) putc(c, stdout);
       }
}