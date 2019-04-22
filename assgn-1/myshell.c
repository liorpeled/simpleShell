#include <errno.h> // need to include library for using perror
#include <fcntl.h> //  need to include for using open()
#include <stdio.h>
#include <stdlib.h> // to eliminate warning using print
#include <string.h> // to elimniate warning using strcmp
#include <sys/types.h>
#include <sys/wait.h> // needs to include library to use wait syscall
#include <unistd.h>
#include <unistd.h> // need to include library for using close()
extern char **getcommand();
// was suppose to pipeline but it did not work so its simply command chaining
int Piping(char **args) {
  int l, m, r;
  int end1[2], end2[2];
  char **temp_args = (char **)malloc(0 * sizeof(char));
  pipe(end1);
  pipe(end2);
  int j = 0;

  for (int i = 0; args[i] != NULL; i++) {

    if (!strcmp(args[i], "|")) {
      i++;

      for (int x = 0; temp_args[x] != NULL; x++) {
        free(temp_args[x]);
        temp_args[x] = NULL;
      }

      j = 0;
    }
    if (!strcmp(args[i], ";")) {
      i++;
      pid_t childPid;
      childPid = fork();

      if (childPid == 0) {
        execvp(temp_args[0], temp_args);
        printf("%s\n", "no such command exist");
        exit(errno);
      } else {
        waitpid(childPid, (void *)0, 0);
      }

      for (int x = 0; temp_args[x] != NULL; x++) {
        free(temp_args[x]);
        temp_args[x] = NULL;
      }

      j = 0;
    }
    temp_args = (char **)realloc(temp_args, j + 1);
    temp_args[j] = args[i];
    if (args[i + 1] == NULL) {
      pid_t childPid;
      childPid = fork();

      if (childPid == 0) {
        execvp(temp_args[0], temp_args);
        printf("%s\n", "no such command exist");
        exit(errno);
      } else {
        waitpid(childPid, (void *)0, 0);
      }
    }
    j++;
  }
  return 0;
}
// redirect function to be used as input, output with over write or just append
int Redirect(char **args) {
  int i = 0;
  char **temp_args = (char **)malloc(0 * sizeof(char));
  pid_t childPid;
  int file_desc;
  int j = 0;
  for (i = 0; args[i] != NULL; i++) {

    // if statmen to check if files is to be used as an input
    if (!strcmp(args[i], "<")) {

      childPid = fork();

      if (childPid == 0) {

        file_desc = open(args[i + 1], O_RDONLY, 0666);
        close(STDIN_FILENO);
        dup(file_desc);
        close(file_desc);
        execvp(temp_args[0], temp_args);
        exit(errno);
      } else {
        waitpid(childPid, (void *)0, 0);
      }
      free(temp_args);
      i++;
    }
    // if statement  to check for files that need to be appended
    if (!strcmp(args[i], ">") && !strcmp(args[i + 1], ">")) {

      childPid = fork();

      if (childPid == 0) {

        file_desc = open(args[i + 2], O_CREAT | O_WRONLY | O_APPEND, 0666);
        close(STDOUT_FILENO);
        dup(file_desc);
        close(file_desc);
        execvp(temp_args[0], temp_args);
        printf("%s\n", "no such command exist");
        exit(errno);
      } else {
        waitpid(childPid, (void *)0, 0);
      }
      free(temp_args);
      return 0;
    }
    // if statement used to check if file is to be over written with new input
    if (!strcmp(args[i], ">")) {
      childPid = fork();

      if (childPid == 0) {

        file_desc = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
        close(STDOUT_FILENO);
        dup(file_desc);
        close(file_desc);
        execvp(temp_args[0], temp_args);
        printf("%s\n", "no such command exist");
        exit(errno);
      } else {
        waitpid(childPid, (void *)0, 0);
      }
      free(temp_args);
    }
    // takes in arguemtents one by one
    temp_args = (char **)realloc(temp_args, i + 1);
    temp_args[i] = args[i];
  }

  return 0;
}
// function that takes in commands with now arguements
int commandsNoArgs(char **args) {

  if (args[1] == NULL) {
    pid_t childPid;
    childPid = fork();
    if (childPid < 0) {
      perror("child process error");
      exit(1);
    }
    if (childPid == 0) {
      execvp(args[0], args);
      printf("%s\n", "no such command exist");
      exit(errno);

    } else {
      wait(0);
    }
  }

  return 0;
}
// function that checks for commands with arguments
int commandsArgs(char **args) {

  if (args[1] != NULL) {
    pid_t childPid;
    childPid = fork();
    if (childPid < 0) {
      perror("child process error");
      exit(1);
    }
    if (childPid == 0) {
      execvp(args[0], args);
      printf("%s\n", "no such command exist"); // tells if execvp fails
      exit(errno);

    } else {
      wait(0);
    }
  }
  return 0;
}

// main file to execute all the functions created
int main() {
  int i;
  char **args;
  int rd;
  int pip; // pip, rd and i, are used as indicators and signals
  int j = 0;
  printf("%s \n", "shell program starts");
  while (1) {
    printf("%s ", "#");
    args = getcommand();
    if (args[0] == NULL) {
      continue;
    }
    if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "Exit") == 0) {

      exit(1);
    }
    if (strcmp(args[0], "cd") == 0) {

      chdir(args[1]);
    }

    // for loop was used to print arugemtns it is used to test for command types
    for (i = 0; args[i] != NULL; i++) {

      // if statments to recognize which type of command
      if (!strcmp(args[i], ">") || !strcmp(args[i], "<")) {
        rd = 1;
      }
      if (!strcmp(args[i], ";") || !strcmp(args[i], "|")) {
        pip = 1;
      }
    }

    if (pip && !rd) {
      Piping(args);

    } else if (rd && !pip) {
      Redirect(args);
    } else {
      commandsNoArgs(args);
      commandsArgs(args);
    }
    pip = 0;
    rd = 0;
  }
}
