#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{	
	int start, status, pid, n;
	int buffer[1];
	if (argc != 4){printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
	/* Parsing of arguments */
  	n = atoi(argv[1]);    		// Cantidad de procesos
    buffer[0] = atoi(argv[2]); 	// Valor inicial
    start = atoi(argv[3]);     	// Proceso que inicia

	if(start>n-1){
		printf("No se puede iniciar desde el proceso %i si solo se tienen %i procesos \n", start, n-1); 
		exit(EXIT_FAILURE);
	}

	printf("Se crear\u00e1n %i procesos, se enviar\u00e1 el caracter %i desde proceso %i \n", n, buffer[0], start);

    int pipes[n][2];
    int pipe_final[2];	// Pipe del proceso start al padre

    if (pipe(pipe_final) == -1){
        perror("pipe_final");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++){
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++){
        pid = fork();

        if (pid < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0){
            // Hijo
            for (int j = 0; j < n; j++){
                if (j != (i - 1 + n) % n) close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }

            if (i != start){
                close(pipe_final[0]);
                close(pipe_final[1]);
            }else{
                close(pipe_final[0]); // Solo escribirá en pipe_final
            }

            int value;
            read(pipes[(i - 1 + n) % n][0], &value, sizeof(int));
            printf("[Hijo %d - PID %d] Recibi: %d\n", i, getpid(), value);
            value++;

            if (i == start){
                write(pipe_final[1], &value, sizeof(int));
            } else {
                write(pipes[i][1], &value, sizeof(int));
            }

            printf("[Hijo %d - PID %d] Envie: %d\n", i, getpid(), value);

            close(pipes[(i - 1 + n) % n][0]);
            close(pipes[i][1]);
            if (i == start) close(pipe_final[1]);
            exit(0);
        }
    }

    // Padre
    close(pipe_final[1]); // Solo lee

    int write_fd = pipes[start][1];
    write(write_fd, &buffer[0], sizeof(int));
    printf("[Padre] Envie: %d al proceso %d\n", buffer[0], start);
    close(write_fd);

    int result;
    read(pipe_final[0], &result, sizeof(int));
    close(pipe_final[0]);

    for (int i = 0; i < n; i++) wait(NULL);

    printf("Resultado final: %d\n", result);
    return 0;
}