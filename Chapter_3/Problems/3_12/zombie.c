#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv)
{
  pid_t pid, pid1;

  printf("Creating child process\n");
  pid = fork();

  if (pid < 0)
  {
    printf("Creating child process failed\n");
    return -1;
  }
  else if (pid == 0)
  {
    pid1 = getpid();
    printf("Child pid: %d\n", pid1);
    printf("Child terminating. BYE!\n");
    exit(1);
  }
  else if (pid > 0)
  {
    //int status;
    //pid = wait(&status);
    //printf("Child pid: %d, status: %d\n", pid, status);

    pid1 = getpid();
    printf("Parent pid: %d\n", pid1);

    /* execute ps -l from the parent */
    execlp("/bin/ps", "ps", "-l", NULL);
  }

  return 0;
}
    
