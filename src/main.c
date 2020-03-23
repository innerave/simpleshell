#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/resource.h>

typedef struct process
{
  char **argv;                /* for exec */
  pid_t pid;                  /* process ID */
  char completed;             /* true if process has completed */
  char stopped;               /* true if process has stopped */
  int status;                 /* reported status value */
} process;

struct process child_p;
int stat_loc;
int foreground;
int change_directory = 0; //rework!!!

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

int parse_line(char*, char**, const int);
int read_line(char*, size_t);

int init_shell(void);
int launch_process(process*, pid_t, int);
int do_process_notification(int);
void put_shell_in_background(pid_t, const int);
void put_shell_in_foreground(pid_t, const int);
void wait_for_process(process*);

int main()
{
    init_shell();

    char to_read[MAX_INPUT];
    char cwd[PATH_MAX];
	char* parsed[50];

    while(1)
    {
        getcwd(cwd, PATH_MAX);
        printf("[%s] simpleshell: $ ", cwd);

        if (read_line(to_read, MAX_INPUT) == -1) return -2;
        if (parse_line(to_read, parsed, 50) == -1) return -3;
        if (parsed[0] == NULL)
        {
            continue;
        }
        if (change_directory == 1) //REWORKKK!!!
        {
            if (chdir(parsed[1]) == -1)
            {
                perror("Change directory");
            }
            continue;
        }

        pid_t pid = fork();

        if (pid == 0)
        
			child_p->argv = parsed;
            launch_process(&child_p, shell_pgid, foreground);
        }
        else
        {
            if (shell_is_interactive)
            {
                child_p.pid = pid;
                setpgid(pid, shell_pgid);
            }
        }

        if (!shell_is_interactive)
            // wait_for_job (j);  ПОКА ХЗ
        else if (foreground)
            put_shell_in_foreground(shell_pgid, 0);
        else
            put_shell_in_background(shell_pgid, 0);

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
        //хз че тут
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

    if (parsed == NULL)
    {
        return 0;
    }

    while (parsed != NULL)
    {
        if (strcmp(parsed, "cd") == 0)
        {
            change_directory = 1;
        }
        output[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
		
		if (index > n) return -1;
    }

    if (strcmp(output[index - 1], "&") == 0)
    {
        foreground = 0;
        output[index - 1] = NULL;
    }
    else
    {
        foreground = 1;
        output[index] = NULL;
    }
    return 0;
}

int init_shell(void)
{
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty (shell_terminal);
    if (!shell_is_interactive) return -1; // error

    /* Loop until we are in the foreground.  */
    while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
        kill (-shell_pgid, SIGTTIN);
    
     /* Ignore interactive and job-control signals.  */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    /* Put ourselves in our own process group.  */
    shell_pgid = getpid();
    if (setpgid (shell_pgid, shell_pgid) < 0)
      {
        perror ("Couldn't put the shell in its own process group");
        exit (1);
      }

    /* Grab control of the terminal.  */
    tcsetpgrp (shell_terminal, shell_pgid);

    /* Save default terminal attributes for shell.  */
    tcgetattr (shell_terminal, &shell_tmodes);
    return 0;
}

int launch_process (process *p, pid_t pgid, int foreground)
{
    if (!shell_is_interactive) return -1; //error

    pid_t pid = getpid();
    if (pgid == 0) pgid = pid;
    setpgid(pid, pgid);
    if (foreground) tcsetpgrp(shell_terminal, pgid);

    /* Set the handling for job control signals back to the default.  */
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);

    execvp(p->argv[0], p->argv);
    perror("execvp");
    exit(1);
}

int do_process_notification(int sig)
{
    // тоже хз
}

void put_shell_in_foreground(pid_t shell, const int cont)
{
    tcsetpgrp (shell_terminal, shell);

    wait_for_process(&child_p);

    /* Put the shell back in the foreground.  */
    tcsetpgrp (shell_terminal, shell_pgid); //stupid shit as we don't use jobs

    /* Restore the shell’s terminal modes.  */
    tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);
}

void put_shell_in_background(pid_t shell, const int cont)
{
    //typically do nothing
}

void wait_for_process(process* p)
{
    int status;
    pid_t pid;

    pid = wait(p->pid, &status, WUNTRACED); //may be do wait while (!job_is_stopped)
}