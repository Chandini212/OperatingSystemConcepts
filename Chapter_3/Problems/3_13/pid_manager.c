#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define MIN_PID    300
#define MAX_PID    5000
#define PID_RANGE  MAX_PID - MIN_PID

int pid_map[PID_RANGE];

int allocate_map (void)
{
  int i = 0;
  for (i = 0; i < PID_RANGE; i++)
  {
    pid_map[i] = 0;
  }
  return 1;
}

int allocate_pid (void)
{
  int i = 0;
  int tmp_pid = 1;

  for (i = 0; i < PID_RANGE; i++)
  {
    if (pid_map[i] == 0)
    {
      pid_map[i] = 1;
      tmp_pid = i + MIN_PID;
      break;
    }
  }

  return tmp_pid;
}

void release_pid (int pid)
{
  if ((pid >= MIN_PID) && (pid <= MAX_PID))
  {
    pid_map[pid - MIN_PID] = 0;
  }
}


int main (int argc, char *argv)
{
  int lReturn;

  allocate_map();
  lReturn = allocate_pid();
  printf("Allocated PID: %d\n", lReturn);
  release_pid(lReturn);

  return 0;
}
