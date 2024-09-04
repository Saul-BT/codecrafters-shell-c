#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

static void cmd_exit(const char *input)
{
  exit(0);
}

static void cmd_echo(const char *input)
{
  printf("%s", input + 5); // input + 5 (aka. input + strlen("echo") + 1)
}

static void cmd_type(const char *input)
{
  const char *arg = input + 5; // input + 5 (aka. input + strlen("type") + 1)
  const int arg_len = (int)strlen(arg) - 1;

  if (!is_builtin(arg))
  {
    printf("%.*s: not found\n", arg_len, arg);
    return;
  }

  printf("%.*s is a shell builtin\n", arg_len, arg);
}

static void *builtins_map[BUILTIN_COUNT][2] = {
  { "exit", &cmd_exit },
  { "echo", &cmd_echo },
  { "type", &cmd_type },
};

static void handle_command(const char *input)
{
  int i = 0;

  for (int i = 0; i < BUILTIN_COUNT; i++)
  {
    if (starts_with(input, builtins_map[i][0]))
    {
      ((void (*)(const char *))builtins_map[i][1])(input);
      return;
    }
  }

  printf("%.*s: command not found\n", (int)strlen(input) - 1, input);
}

int main()
{
  char input[100];

  while (true)
  {
    printf("$ ");
    fflush(stdout);

    fgets(input, 100, stdin);
    handle_command((const char *)input);
  }
  return 0;
}
