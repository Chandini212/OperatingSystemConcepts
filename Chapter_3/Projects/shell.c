#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef int bool;

#define true 1
#define false 0

#define SHELL_MAX_LINE    1024 /* The maximum length command */
#define SHELL_HIST_SIZE   10
#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM   " \t\r\n\a"

/* global variable to check parent and child process concurrency */
bool conc = false;
/* global variable to point to the last command executed */
int cur_pos = -1;
/* global variable storing the history of commands executed */
char *history[SHELL_HIST_SIZE];
int cur_bufsize = SHELL_TOK_BUFSIZE;

/* Launch a program */
int shell_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if(pid == 0)
  { /* child process */
    if (execvp(args[0], args) == -1)
    {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  }
  else if (pid > 0)
  { /* parent process */
    if (!conc)
    {
      do
      {
        wpid =  waitpid(pid, &status, WUNTRACED);
      } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }
  else
  { /* error forking */
    perror("shell");
  }

  conc = false;
  return 1;
}

char **shell_split_line (char *line)
{
  char **tokens = malloc(cur_bufsize * sizeof(char*));
  int position = 0;
  char *token;

  token = strtok(line, SHELL_TOK_DELIM);
  while (token != NULL)
  {
    tokens[position] = token;
    position++;

    if(position >= cur_bufsize)
    {
      cur_bufsize += SHELL_TOK_BUFSIZE;
      tokens = realloc(tokens, cur_bufsize * sizeof(char*));
      if(!tokens)
      {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SHELL_TOK_DELIM);
  }

  if ((position > 0) && (strcmp(tokens[position - 1], "&") == 0))
  {
    conc = true;
    tokens[position - 1] = NULL;
  }

  tokens[position] = NULL;
  return tokens;
} 

int shell_history (char **args)
{
  if ((cur_pos == -1) || (history[cur_pos] == NULL))
  {
    fprintf(stderr, "No commands in history\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(args[0], "history") == 0)
  {
    int last_pos = 0, position = cur_pos, count = 0;

    if ((cur_pos != SHELL_HIST_SIZE) && (history[cur_pos + 1] != NULL))
    {
      last_pos = cur_pos + 1;
    }

    count = (cur_pos - last_pos + SHELL_HIST_SIZE) % SHELL_HIST_SIZE + 1;

    while (count > 0)
    {
      char *command = history[position];
      printf("%d %s\n", count, command);
      position = position - 1;
      position = (position + SHELL_HIST_SIZE) % SHELL_HIST_SIZE;
      count --;
    }
  }
  else
  {
    char **cmd_args;
    char *command;
    if (strcmp(args[0], "!!") == 0)
    {
      command = malloc(sizeof(history[cur_pos]));
      strcat(command, history[cur_pos]);
      printf("%s\n", command);
      cmd_args = shell_split_line(command);
      int i;

      return shell_launch(cmd_args);
    }
    else if (args[0][0] == '!')
    {
      if (args[0][1] == '\0')
      {
        fprintf(stderr, "Expected arguments for \"!\"\n");
        exit(EXIT_FAILURE);
      }
      /* position of the command to execute */
      int offset = args[0][1] - '0';
      int next_pos = (cur_pos + 1) % SHELL_HIST_SIZE;

      if ((next_pos != 0) && (history[cur_pos + 1] != NULL))
      {
        offset = (cur_pos + offset) % SHELL_HIST_SIZE;
      }
      else
      {
        offset--;
      }

      if (history[offset] == NULL)
      {
        fprintf(stderr, "No such command in history\n");
        exit(EXIT_FAILURE);
      }

      command = malloc(sizeof(history[cur_pos]));
      strcat(command, history[offset]);
      cmd_args = shell_split_line(command);
      int i;

      return shell_launch(cmd_args);
    }
    else
    {
      perror("shell");
    }
  }
}

int shell_execute (char *line)
{
  int i;

  char **args = shell_split_line(line);

  if (args[0] == NULL)
  { /* empty command was entered */
    return 1;
  }
  else if ((strcmp(args[0], "history") == 0) || (strcmp(args[0], "!!") == 0) || (args[0][0] == '!'))
  {
    return shell_history(args);
  }

  cur_pos = (cur_pos + 1) % SHELL_HIST_SIZE;
  history[cur_pos] = malloc(cur_bufsize * sizeof(char));
  char **temp_args = args;
  int count=0;

  while (*temp_args != NULL)
  {
    strcat(history[cur_pos], *temp_args);
    strcat(history[cur_pos], " ");
    temp_args++;
  }

  if (cur_pos > 0)
  {
    printf("Inserted %s\n", history[cur_pos-1]);
  }

  return shell_launch(args);
}

char *shell_read_command (void)
{
  cur_bufsize = SHELL_MAX_LINE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * cur_bufsize);
  int c;

  if (!buffer)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while(1)
  {
    /* Read a character */
    c = getchar();

    if (c == EOF || c == '\n')
    {
      buffer[position] = '\0';
      return buffer;
    }
    else
    {
      buffer[position] = c;
    }
    position++;

    /* If buffer exceeded, reallocate buffer */
    if (position >= cur_bufsize)
    {
      cur_bufsize += SHELL_MAX_LINE;
      buffer = realloc(buffer, cur_bufsize);
      if (!buffer)
      {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void shell_loop (void)
{
  char *command;
  int status;

  do
  {
    printf(">");
    command = shell_read_command();
    status = shell_execute(command);

    free(command);
  } while (status);
}

int main(void)
{
  shell_loop();
  return EXIT_SUCCESS;
}
