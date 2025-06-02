#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_COMMANDS 200
#define MAX_ARGS     64

int main() {
    char *command = NULL;
    size_t len = 0;

    while(getline(&command, &len, stdin) != -1){
        size_t n = strlen(command);
        if(n > 0 && command[n-1] == '\n'){
            command[--n] = '\0';
        }
        if(n == 0) continue;

        if(command[0] == '|' || command[n-1] == '|' || strstr(command, "||") || strstr(command, "| |")) {
            continue;
        }

        char *commands[MAX_COMMANDS];
        int command_count = 0;
        char *saveptr;
        char *tok = strtok_r(command, "|", &saveptr);
        while(tok && command_count < MAX_COMMANDS){
            while(isspace((unsigned char)*tok)) tok++;
            if(*tok == '\0'){
                command_count = 0;
                break;
            }
            commands[command_count++] = tok;
            tok = strtok_r(NULL, "|", &saveptr);
        }
        if (command_count <= 0) continue;
        int pipefd[2*(command_count-1)];
        for (int i = 0; i < command_count-1; ++i) {
            if (pipe(pipefd + 2*i) < 0) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < command_count; ++i) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                // redirigir stdin
                if (i > 0) {
                    dup2(pipefd[2*(i-1)], STDIN_FILENO);
                }
                // redirigir stdout
                if (i < command_count-1) {
                    dup2(pipefd[2*i + 1], STDOUT_FILENO);
                }
                // cerrar todos los pipes
                for (int j = 0; j < 2*(command_count-1); ++j) {
                    close(pipefd[j]);
                }

                char *args[MAX_ARGS + 1];
                char *cmd = commands[i];
                int k = 0;
                while (*cmd) {
                    while (isspace((unsigned char)*cmd)) cmd++;
                    if (*cmd == '\0') break;
                    if (k == MAX_ARGS){exit(EXIT_FAILURE);}

                    if (*cmd == '"' || *cmd == '\'') {
                        char quote = *cmd++;
                        args[k++] = cmd;
                        while (*cmd && *cmd != quote) cmd++;
                        if (*cmd) {
                            *cmd++ = '\0';
                        } else {
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        args[k++] = cmd;
                        while (*cmd && !isspace((unsigned char)*cmd)) cmd++;
                        if (*cmd) {
                            *cmd++ = '\0';
                        }
                    }
                }
                args[k] = NULL;

                if (strcmp(args[0], "echo") == 0) {
                    // calcular longitud total de args[1..]
                    int total = 0;
                    for (int j = 1; args[j]; ++j) {
                        total += strlen(args[j]);
                    }
                    if (total == 0) {
                        exit(EXIT_SUCCESS);
                    }
                    for (int j = 1; args[j]; ++j) {
                        write(STDOUT_FILENO, args[j], strlen(args[j]));
                        if (args[j+1]) write(STDOUT_FILENO, " ", 1);
                    }
                    write(STDOUT_FILENO, "\n", 1);
                    exit(EXIT_SUCCESS);
                }
                execvp(args[0], args);
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < 2*(command_count-1); ++i) {
            close(pipefd[i]);
        }
        for (int i = 0; i < command_count; ++i) {
            wait(NULL);
        }
    }
    free(command);
    return 0;
}
