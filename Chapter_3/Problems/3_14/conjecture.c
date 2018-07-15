#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int conjecture (int num)
{
  int tmp;

  if (num > 0)
  {
    if ((num % 2) == 0)
    {
      tmp = num / 2;
    }
    else
    {
      tmp = (3 * num) + 1;
    }
  }

  return tmp;
}

int main (int argc, char *argv[])
{
  int tmp;
  pid_t pid;

  if (argc == 2)
  {
    argv++;
    tmp = atoi(*argv);
    printf("Number: %d\n", tmp);
  }

  pid = fork();
  if (pid < 0)
  {
    printf("Creating child failed\n");
    return -1;
  }
  else if (pid == 0)
  {
    while (tmp >= 1)
    {
      printf("%d, ",tmp);
      tmp = conjecture(tmp);
      if (tmp == 1)
      {
        printf("%d",tmp);
        break;
      }
    }
    printf("\n");
  }
  else
  {
    int status;
    pid = wait(&status);
  }

  return 0;
}
