#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

pid_t child_pid;
int stat_loc;

int main(){
    char to_read[MAX_INPUT];
    char parsed[50][MAX_INPUT];
    
    while(1){
        printf("simpleshell>");
        if (read_line(to_read, MAX_INPUT) == -1) return -2;
        if (parse_line(parsed,to_read) == -1) return -3;

        child_pid = fork();
        if (child_pid == 0) {
            /* Never returns if the call is successful */
            execvp(command[0], command);
            printf("This won't be printed if execvp is successul\n");
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

    }
/*Т.к. не может выйти из цикла*/
    return -1;
}

int read_line(char *line, size_t size){
    char c;
    int pos = 0;

    while (1)
    {
        c = getchar();
        if (c == EOF || c == '\n'){
            line[pos] = '\0';
            return 0;
        } else {
            line[pos] = c;
        }
        pos++;
        if (pos > size) {
            printf("Error: exided space\n");
            return -1;
        }
    }
    return -2;
}

int get_input(char *input, char **output, const int n) {
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        strcpy(output[index], parsed);
        index++;
        if (index > n) {
            printf("Error: too much args\n");
            return -1;
        }
        parsed = strtok(NULL, separator);
    }

    return 0;
}