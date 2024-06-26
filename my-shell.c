#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

#define MAX_INPUT_LENGTH 100
#define HISTORY_SIZE 10

#define MAX_ALIASES 100
#define MAX_ALIAS_LENGTH 50

typedef struct{
    char alias[MAX_ALIAS_LENGTH];
    char command[MAX_INPUT_LENGTH];
} Alias;

Alias aliases[MAX_ALIASES];
int num_aliases = 0;

void print_prompt();
void read_shell_input(char *command, char *parameters[]);
void change_directory(char *path);
void exit_shell();
void process_command_history(Queue history);
void create_alias(char **parameters);
char** get_alias_command(char *alias);
void execute_command_external(char *command, char *parameters[]);

int main() {
    Queue history;
    init(&history); // inicializa a fila do historico

    while(1) {
        print_prompt(); 

        char command[MAX_INPUT_LENGTH];
        char *parameters[MAX_INPUT_LENGTH];

        read_shell_input(command, parameters);

        // COMANDOS INTERNOS:

        // verifica se o comando é um alias e substitui pelo comando se existir
        char** alias_command = get_alias_command(command);
        if (alias_command != NULL) { 
            execute_command_external(alias_command[0], alias_command);
        }
        if (!strcmp(command, "exit")) { 
            exit_shell();
        }
        else if (!strcmp(command, "cd")) { 
            change_directory(parameters[1]);
        }
        else if (!strcmp(command, "history")) { 
            process_command_history(history);
        }
        else if (!strcmp(command, "alias")) { 
            create_alias(parameters);
        }

        // COMANDOS EXTERNOS:
        else {
            execute_command_external(command, parameters);
        }

        // ADICIONA COMANDO AO HISTÓRICO:
        enqueue(history, command);
    }

    return 0;
}

void print_prompt() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char* user = getenv("USER");

    char* host = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    gethostname(host, MAX_INPUT_LENGTH);

    printf("%s@%s[%02d:%02d:%02d] $ ", user, host, tm.tm_hour, tm.tm_min, tm.tm_sec);

    free(host);
}

void read_shell_input(char *command, char *parameters[]) {
    char input[100];
    fgets(input, sizeof(input), stdin); // lê toda a linha de entrada
    sscanf(input, "%s", command); // extrai o comando da linha de entrada

    if (strcmp(command, "alias") == 0) {
        // se o comando for alias, tudo depois de alias é um único parâmetro
        parameters[0] = strstr(input, command) + strlen(command) + 1;
        parameters[1] = NULL;
    } else {
        // senao divide a entrada em tokens
        int i = 0;
        char *token = strtok(input, " \n"); // quebra a linha em tokens separados por espaço ou nova linha
        while (token != NULL && i < 9) { // limite de 9 parâmetros
            parameters[i++] = token; // adicionar token aos parâmetros
            token = strtok(NULL, " \n"); // próximo token
        }
        parameters[i] = NULL; // último elemento dos parâmetros deve ser NULL para execvp()
    }
}

void change_directory(char *path) {
    if (chdir(path) != 0) {
        printf("error: failed to change directory '%s'\n", path);
    }
}

void exit_shell() {
    exit(0);
}

void execute_command_external(char *command, char *parameters[]) {
    pid_t pid = fork(); 

    if (pid == -1) { // fork failed
        perror("fork");
        exit(1);
    }
    else if (pid == 0) { // child process
        if (execvp(command, parameters) == -1) { // execute the external command
            perror("execvp");
            exit(1);
        }
    }
    else { // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) { // wait for the child process to finish
            perror("waitpid");
            exit(1);
        }
    }
}

void process_command_history(Queue history) {
    int queue_size = size(history);
    if (is_empty(history)) {
        printf("No commands in history.\n");
    } else if(is_full(history)) {
        printf("error: queue is full, clearing the command history...\n");
        for (int i = 0; i < queue_size; i++){
            dequeue(history);
        }
    } else {
        printf("Command history:\n");
        for (int i = 0; i < queue_size; i++) {
            char* command = dequeue(history);
            printf("[%d]: %s\n", i + 1, command);
            enqueue(history, command);
        }
    }
}

void create_alias(char **parameters) {
    char *alias, *command;
    char *parameters_copy = strdup(parameters[0]); // copia os parâmetros para evitar alterações
    alias = strtok(parameters_copy, "="); // dvide a string no sinal de igual
    command = strtok(NULL, "\""); // o comando dentro das aspas duplas

    if (alias == NULL || command == NULL) {
        printf("error: invalid alias command\n");
        free(parameters_copy);
    }else if (strlen(alias) >= MAX_ALIAS_LENGTH || strlen(command) >= MAX_INPUT_LENGTH) {
        printf("error: alias or command too long\n");
        free(parameters_copy);
    }

    // inicializa struct do alias e define como vazia
    Alias new_alias;
    memset(&new_alias, 0, sizeof(Alias));

    // guarda o alias e o comando na struct
    strcpy(new_alias.alias, alias);
    strcpy(new_alias.command, command);

    // adiciona alias no array
    aliases[num_aliases] = new_alias;
    num_aliases++;

    printf("Alias: %s, Command: %s\n", aliases[num_aliases-1].alias, aliases[num_aliases-1].command);
    free(parameters_copy);
}

char** get_alias_command(char *alias) {
    for (int i = 0; i < num_aliases; i++) {
        if (strcmp(aliases[i].alias, alias) == 0) {
            char **command_parts = malloc(MAX_INPUT_LENGTH * sizeof(char *)); // armazena as partes do comando
            char *part = strtok(aliases[i].command, " "); // divide o comando em partes por espaço
            int j = 0; 
            while (part != NULL) {
                command_parts[j++] = strdup(part); // armazena cada parte do comando na matriz
                part = strtok(NULL, " "); // pega prox parte
            }
            command_parts[j] = NULL; // fim do comando
            return command_parts;
        }
    }
    return NULL;
}