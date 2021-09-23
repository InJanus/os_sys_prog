#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{ pid_t child;
  int a = 2;
  int b = 2;
  int c = 5;
  int status;
  child = fork();

  if (child == 0)
     { printf("%d + %d = %d\n", a, b, c);
     }
  else
     { c = a + b;
       wait(&status);
       exit(0);
     }
}