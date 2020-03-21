#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char *argv[] = {"ls", "-l", "-h", "-a", NULL};
    execvp(argv[0], argv);

    return 0;
}