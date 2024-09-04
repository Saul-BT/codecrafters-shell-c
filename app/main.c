#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

static void handle_command(const char *input)
{
  int i = 0;
  static void *builtins[2][2] = {
    { "exit", &cmd_exit },
    { NULL },
  };

  while (builtins[i][0])
  {
    if (starts_with(input, builtins[i][0]))
    {
      ((void (*)(const char *))builtins[i][1])(input);
      return;
    }
    i++;
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
