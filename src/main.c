#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>

pid_t child_pid;
int stat_loc;
char* pwd;
int background_work = 0;
int change_directory = 0;

int parse_line(char*, char**, const int);
int read_line(char*, size_t);

int main()
{
    char to_read[MAX_INPUT];
    char* parsed[50];
    while(1)
    {
        pwd = getenv("PWD");
        printf("[%s] simpleshell: > ", pwd);
        if (read_line(to_read, MAX_INPUT) == -1) return -2;
        if (parse_line(to_read, parsed, 50) == -1) return -3;
        if (change_directory == 1)
        {
            chdir(parsed[1]);
        }
        else if (parsed[0][0] == '$')
        {
            printf("%s", getenv(parsed));
        }
        else
        {
            child_pid = fork();
            if (child_pid == 0)
            {
                /* Never returns if the call is successful */
                execvp(parsed[0], parsed);
                printf("Something went wrong\n");
            }
            else
            {
                signal(SIGINT, SIG_IGN);
                if (background_work == 1)
                {
                    waitpid(child_pid, &stat_loc, WNOHANG);
                    printf("Child PID: %d\n", child_pid);
                }
                else
                {
                    waitpid(child_pid, &stat_loc, WUNTRACED);
                }
            }
        }
    }
    /* Т.к. не может выйти из цикла */
    return -1;
}

int read_line(char *line, size_t size)
{
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

int parse_line(char* input, char** output, const int n)
{
    char* separator = " ";
    char* parsed;
    int index = 0;
    change_directory = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL)
    {
        if (strcmp(parsed, "cd") == 0)
        {
            change_directory = 1;
        }
        output[index] = parsed;
        index++;
        if (index > n)
        {
            printf("Error: too much args\n");
            return -1;
        }
        parsed = strtok(NULL, separator);
    }

    if (strcmp(output[index - 1], "&") == 0)
    {
        background_work = 1;
        output[index - 1] = NULL;
    }
    else
    {
        background_work = 0;
        output[index] = NULL;
    }
    return 0;
}
