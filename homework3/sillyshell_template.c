#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/**************************************************************/
/* Text Processing / Parsing Routines                         */
/**************************************************************/

// Function: void parse(char *line, char **argv)
// Purpose : This function takes in a null terminated string pointed to by 
//           <line>.  It also takes in an array of pointers to char <argv>.
//           When the function returns, the string pointed to by the 
//           pointer <line> has ALL of its whitespace characters (space, 
//           tab, and newline) turned into null characters ('\0').  The
//           array of pointers to chars will be modified so that the zeroth
//           slot will point to the first non-null character in the string
//           pointed to by <line>, the oneth slot will point to the second 
//           non-null character in the string pointed to by <line>, and so
//           on. In other words, each subsequent pointer in argv will point
//           to each subsequent "token" (characters separated by white space)
//           IN the block of memory stored at the pointer <line>.  Since all
//           the white space is replaced by '\0', every one of these "tokens"
//           pointed to by subsequent entires of argv will be a valid string
//           The "last" entry in the argv array will be set to NULL.  This 
//           will mark the end of the tokens in the string.
//           
void  parse(char *line, char **argv){
     // We will assume that the input string is NULL terminated.  If it
     // is not, this code WILL break.  The rewriting of whitespace characters
     // and the updating of pointers in argv are interleaved.  Basically
     // we do a while loop that will go until we run out of characters in
     // the string (the outer while loop that goes until '\0').  Inside
     // that loop, we interleave between rewriting white space (space, tab,
     // and newline) with nulls ('\0') AND just skipping over non-whitespace.
     // Note that whenever we encounter a non-whitespace character, we record
     // that address in the array of address at argv and increment it.  When
     // we run out of tokens in the string, we make the last entry in the array
     // at argv NULL.  This marks the end of pointers to tokens.  Easy, right?
    
     while (*line != '\0') // outer loop.  keep going until the whole string is read
        { // keep moving forward the pointer into the input string until
          // we encounter a non-whitespace character.  While we're at it,
          // turn all those whitespace characters we're seeing into null chars.

          while (*line == ' ' || *line == '\t' || *line == '\n' || *line == '\r'){
            *line = '\0';     
            line++;
          }

          // If I got this far, I MUST be looking at a non-whitespace character,
          // or, the beginning of a token.  So, let's record the address of this
          // beginning of token to the address I'm pointing at now. (Put it in *argv)
          // then we'll increment argv so that the next time I store an address, it 
          // will be in the next slot of the array of integers.

          *argv++ = line;          /* save the argument position     */

          // Ok... now let's just keep incrementing the input line pointer until
          // I'm looking at whitespace again.  This "eats" the token I just found
          // and sets me up to look for the next.

          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n' && *line !='\r') 
               line++;             /* skip the argument until ...    */
        }

     // Heh, I ran out of characters in the input string.  I guess I'm out of tokens.
     // So, whatever slot of the array at argv I'm pointing at?  Yeah, put a NULL
     // there so we can mark the end of entries in the table.

     *argv = NULL;                 /* mark the end of argument list  */
}


/**************************************************************/
/* Signal Handling Routines                                   */
/**************************************************************/
void ignore_signal_parent(int signum)
{
  //do nothing
  //printf("\nI was sent signal number %d, but I choose to ignore it\n", signum);
  //send kill signal to child?
  //printf("%i", signum);
}

/**************************************************************/
/* Shell Command Processing Routines                          */
/**************************************************************/

void execute(char **argv, char *raw_line_input)
{
     pid_t  pid,wpid;
     
     if ((pid = fork()) < 0) 
        { /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
        } else 
     
     if (pid == 0) 
       { /* for the child process:         */
         /* The child is going to say what it should have done, then exit */
        //printf("The command line <%s> should have been executed here\n", raw_line_input);
         execvp(*argv, argv);
         printf("The command %s is not recognized\n", *argv);
         exit (0); 
       } else { /* for the parent:      */
         // If we get here at all, this is the parent shell.  Maybe this would be a good
         // place to do a "wait" of some kind.  Be careful, though, there's different
         // kinds of waits...
        int *status;
        waitpid(pid,status,WUNTRACED);
        //wait(status);
        //printf("%i\n", *status);
        if(*status == SIGINT){
          //sigint was called on child process
          printf("\nSIGINT called on %i\n", pid);
          //true means the call failed
          //do nothing
        }else if(*status == SIGTSTP || *status == 5247){
          //sigint was called on child process
          printf("\nSIGTSTP called on %i\n", pid);
          //true means the call failed
          //do nothing
        }else if(*status == 0){
          printf("%i terminated normally\n", pid);
        }else{
          printf("Process %i returned status %i\n", pid, *status);
        }
         
         return;
       }
     return;
}

int main(int argc, char **argv, char **envp)
{    

     int              status;
     int              job;

     char  line[1024];   // This is the string buffer that will hold
                         // the string typed in by the user.  This 
                         // string will be parsed.  The shell will do
                         // what it needs to do based on the tokens it
                         // finds.  Note that a user may NOT type in 
                         // an input line of greater than 1024 characters
                         // because that's the size of the array.
                         
     char raw_line[1024];  // This will hold an unmodified copy of the user's
                           // input at the command line.
                                          
     char  *line_argv[64];    // This is a pointer to an array of 64 pointers to
                              // char, or, an array of pointers to strings. 
                              // after parsing, this array will hold pointers
                              // to memory INSIDE of the string pointed to by 
                              // the pointer line.  argv[0] will be the string
                              // version of the first token inside of line... 
                              // argv[1] will be the second... and so on... 
                              // See the routine parse() for details.

     char shell_prompt[15]; // This string will hold the shell prompt string

     int index = 0;
     
     int job_number_int;
     
     // set the default prompt
     strcpy(shell_prompt, "SillyShell");

     // Set signal handlers if you think you need to....
     // set them here?
      signal(SIGINT, ignore_signal_parent);
      signal(SIGTSTP, ignore_signal_parent);
     while (1) 
       {  // Maybe once per processing loop you could try to check if there's
          // status signals coming from children, and then deal with them?
          // Might this be a good place to do that?
          printf("%s> ",shell_prompt);  // display the shell prompt

          if(fgets(line, 1024, stdin) == NULL)
             { printf("\n");
               exit(0);
             };  
             
          line[strlen(line)-1]='\0'*status == 5247; // This is a dirty hack.  Figure it out maybe?
          strncpy(raw_line, line, 1024);


          if ((*line != '\0') && (*line > 31)) // If something was actually typed, then do something...
            { 
            
              // First, get all the addresses of all of the tokens inside the input line
              
              parse(line, line_argv);     //   parse the line to break it into token references

              // Check the first token to see if there are any built in commands
              // we want to handle directly and will do it with an "if/then/else" ladder.
              // if we hit the end of the ladder, we assume the command line was requesting
              // an external program be run as a child process and do that....  
     
              if (strcmp(line_argv[0], "exit")         == 0) exit(0); else
              
              if (strcmp(line_argv[0], "done")         == 0) exit(0); else
              
              if (strcmp(line_argv[0], "quit")         == 0) exit(0); else
              
              if (strcmp(line_argv[0], "cd")  == 0 && line_argv[2] == NULL) {
                //built in cd command
                chdir(*(line_argv+1));
              } else if (strcmp(line_argv[0], "pwd")  == 0) {
                //built in pwd command
                //i could just use the default command for this and let it go to the forking
                char *buff = (char *)malloc(FILENAME_MAX * sizeof(char)); //create string buffer to hold path
                getcwd( buff, FILENAME_MAX );
                printf("%s\n", buff);
              } else if (strcmp(line_argv[0], "printenv")  == 0) { 
                
                // printf("This should have printed all the environment variables\n");
                char **store = envp;
                while(*envp){
                  printf("%s\n", *envp);
                  envp++;
                }
                envp = store;
              }else if(strcmp(line_argv[0], "newprompt") == 0) {
                if (line_argv[1] != NULL){
                    strncpy(shell_prompt, line_argv[1], 15); 
                }else{
                    strncpy(shell_prompt, "SillyShell", 15);
                }
              } else 
                                                          
                                                          
               // If we got this far, it's not a built-in.  Process the line as if the first
               // token is the name of a program somewhere in the current path environment
               // variable. execute() is the routine that will deal with launching any 
               // children.  You'll have to replace my stub code in execute() with something that
               // does the job that's needed.                                              
              
               execute(line_argv, raw_line);
               
            }
     }
}