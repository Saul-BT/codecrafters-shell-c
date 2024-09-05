#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUILTIN_COUNT 5

static char *builtins[BUILTIN_COUNT] = {
  "exit",
  "echo",
  "type",
  "pwd",
  "cd",
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

static char *get_bin_path(const char *cmd, char **envp)
{
  char *cmd_cpy = strdup(cmd);
  char *cmd_name = strtok(cmd_cpy, " \n");
  const int cmd_name_len = (int)strlen(cmd_name);

  char *path = getenv("PATH");
  if (!path)
  {
    printf("%.*s: not found\n", cmd_name_len, cmd_name);
    free(cmd_cpy);
    return NULL;
  }

  const char *path_delim = ":";
  char tmp[1000];
  char *path_dirs = strdup(path);
  char *path_dir = strtok(path_dirs, path_delim);
  while (path_dir)
  {
    strcpy(tmp, path_dir);
    strcat(tmp, "/");
    strncat(tmp, cmd_name, strlen(cmd_name));
    if (access(tmp, X_OK) == 0)
    {
      free(path_dirs);
      free(cmd_cpy);
      return strdup(tmp);
    }

    path_dir = strtok(NULL, path_delim);
  }

  free(path_dirs);
  free(cmd_cpy);
  return NULL;
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

  if (is_builtin(arg))
  {
    printf("%.*s is a shell builtin\n", arg_len, arg);
    return;
  }

  char *bin_path = get_bin_path(arg, envp);
  if (bin_path)
  {
    printf("%.*s is %s\n", (int)strlen(arg) - 1, arg, bin_path);
    free(bin_path);
    return;
  }

  printf("%.*s: not found\n", arg_len, arg);
}

static void cmd_pwd(const char *input)
{
  char *pwd = calloc(100, sizeof(char));

  getcwd(pwd, 100);
  printf("%s\n", pwd);
  free(pwd);
}

static void cmd_cd(const char *input)
{
  char path[100];
  sprintf(path, "%.*s", (int)strlen(input + 3) - 1, input + 3);

  if (chdir(path))
    printf("cd: %s: No such file or directory\n", path);
}

static void *builtins_map[BUILTIN_COUNT][2] = {
  { "exit", &cmd_exit },
  { "echo", &cmd_echo },
  { "type", &cmd_type },
  { "pwd",  &cmd_pwd  },
  { "cd",   &cmd_cd   },
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

  char *bin_path = get_bin_path(input, envp);
  if (bin_path)
  {
    free(bin_path);
    system(input);
    return;
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
