#include <stdio.h>

static int strlen_until(char *str, char c)
{
  int len = 0;

  while (str[len] != c)
    len++;

  return len;
}

int main() {
  printf("$ ");
  fflush(stdout);

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin);
  printf("%.*s: command not found\n", strlen_until(input, '\n'), input);
  return 0;
}
