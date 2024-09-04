#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int strlen_until(char *str, char c)
{
  int len = 0;

  while (str[len] != c)
    len++;

  return len;
}

static bool starts_with(char *str, char *suffix)
{
  int i = 0;


  while (str[i] && suffix[i] && str[i] == suffix[i])
    i++;

  return (str[i] == ' ' || str[i] == '\n');
}

static void handle_command(char *input)
{
  if (starts_with(input, "exit")) {
    exit(0);
  } else {
    printf("%.*s: command not found\n", strlen_until(input, '\n'), input);
  }
}

int main()
{
  char input[100];

  while (true)
  {
    printf("$ ");
    fflush(stdout);

    fgets(input, 100, stdin);
    handle_command(input);
  }
  return 0;
}
