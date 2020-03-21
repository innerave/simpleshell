#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/wait.h>

pid_t child_pid;
int stat_loc;

int parse_line(char*, char**, const int);
int read_line(char*, size_t);

int main()
{
    char to_read[MAX_INPUT];
    char* parsed[50];

    while(1){
        printf("simpleshell>");
        if (read_line(to_read, MAX_INPUT) == -1) return -2;
        if (parse_line(to_read, parsed, 50) == -1) return -3;
        child_pid = fork();
        if (child_pid == 0)
        {
            /* Never returns if the call is successful */
            execvp(parsed[0], parsed);
            printf("This won't be printed if execvp is successul\n errno=%d\n", errno);
        }
        else
        {
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
        if (c == EOF || c == '\n')
        {
            line[pos] = '\0';
            return 0;
        }
        else
        {
            line[pos] = c;
        }
        pos++;
        if (pos > (int)size)
        {
            printf("Error: exided space\n");
            return -1;
        }
    }
    return -2;
}

int parse_line(char* input, char** output, const int n) {
    char* separator = " ";
    char* parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL)
    {
        output[index] = parsed;
        index++;
        if (index > n)
        {
            printf("Error: too much args\n");
            return -1;
        }
        parsed = strtok(NULL, separator);
    }
    output[index] = NULL;
    return 0;
}
