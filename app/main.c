#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUILTIN_COUNT 3

static char *builtins[BUILTIN_COUNT] = {
  "exit",
  "echo",
  "type",
};

static bool is_builtin(const char *cmd)
{
  for (int i = 0; i < BUILTIN_COUNT; i++)
    if (!strncmp(cmd, builtins[i], strlen(cmd) - 1))
      return true;

  return false;
}

static bool starts_with(const char *str, char *suffix)
{
  int i = 0;


  while (str[i] && suffix[i] && str[i] == suffix[i])
    i++;

  return (str[i] == ' ' || str[i] == '\n');
}

static char *get_path_dirs(char **envp)
{
  int i = 0;

  while (strncmp(envp[i], "PATH=", 5))
    i++;
  
  char *path_dirs = envp[i] + 5;

  return path_dirs;
}

static void cmd_exit(const char *input)
{
  exit(0);
}

static void cmd_echo(const char *input)
{
  printf("%s", input + 5); // input + 5 (aka. input + strlen("echo") + 1)
}

static void cmd_type(const char *input, char **envp)
{
  const char *arg = input + 5; // input + 5 (aka. input + strlen("type") + 1)
  const int arg_len = (int)strlen(arg) - 1;
  char *path_dirs = get_path_dirs(envp);

  if (is_builtin(arg))
  {
    printf("%.*s is a shell builtin\n", arg_len, arg);
    return;
  }

  const char *path_delim = ":";
  char *path_dir = strtok(path_dirs, path_delim);

  while (path_dir)
  {
    char tmp[1000];
    strcpy(tmp, path_dir);
    strcat(tmp, "/");
    strncat(tmp, arg, strlen(arg) - 1);

    if (access(tmp, F_OK) == 0)
    {
      printf("%.*s is %s\n", (int)strlen(arg) - 1, arg, tmp);
      return;
    }

    path_dir = strtok(NULL, path_delim);
  }

  printf("%.*s: not found\n", arg_len, arg);
}

static void *builtins_map[BUILTIN_COUNT][2] = {
  { "exit", &cmd_exit },
  { "echo", &cmd_echo },
  { "type", &cmd_type },
};

static void handle_command(const char *input, char **envp)
{
  int i = 0;

  for (int i = 0; i < BUILTIN_COUNT; i++)
  {
    if (starts_with(input, builtins_map[i][0]))
    {
      ((void (*)(const char *, char **))builtins_map[i][1])(input, envp);
      return;
    }
  }

  printf("%.*s: command not found\n", (int)strlen(input) - 1, input);
}

int main(int _argc, char **_argv, char **envp)
{
  char input[100];

  while (true)
  {
    printf("$ ");
    fflush(stdout);

    fgets(input, 100, stdin);
    handle_command((const char *)input, envp);
  }
  return 0;
}
