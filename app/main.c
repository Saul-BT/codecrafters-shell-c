#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static int strlen_until(char *str, char c)
{
  int len = 0;

  while (str[len] != c)
    len++;

  return len;
}

int main()
{
  while (true)
  {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);
    printf("%.*s: command not found\n", strlen_until(input, '\n'), input);
  }
  return 0;
}
