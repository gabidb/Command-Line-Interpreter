#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char **read_arguments(char *command, int *block);
char execute_command(char **arguments, int block);

int main(void) {
    char command[128];
    char **arguments;
    int status;
    int block = 0;

    do {

        printf("ggg190>");
        block = 0;
        if (fgets(command,128,stdin) == NULL) {  /* get command from the console */
            printf("\n");
            break;
        }
        arguments = read_arguments(command, &block);
        
        status = execute_command(arguments, block);
        free(arguments);

    }while(status);
}

char **read_arguments(char *command, int *block) {
    char **tokens = malloc(128);
    char *token;
   
   /* get the first argument */
    token = strtok(command, " \t\r\n\a");
   
   /* walk through other arguments */
   int i = 0;
   while ( token != NULL ) {  
       if (strcmp(token,"&")) {
            tokens[i] = token;
            i++;
       } else {
           *block = 1;
       }
      token = strtok(NULL, " \t\r\n\a");
   }
    tokens[i] = NULL;
   return tokens;
}

char execute_command(char **arguments, int block) {
    int pid = fork();   /* creating a child process */
    int status;

    if (pid == 0) {      /* child process */

        if (execvp(arguments[0], arguments) == -1) {  /* Execute command */
            printf("Error with executing command \n");
        }

    } else if (pid < 0) {

        printf("error with creating child process \n");

    } else {      /* parent process */
        if (!block) {
            waitpid(pid, NULL, 0); /* wait until child finishes */
        } else {
            printf("Background, pid:%i, command:%s\n",pid,arguments[0]); /* Print a message about background process */
        }
    }
    return 1;
}



