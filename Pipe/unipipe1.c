#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define BUFSIZE 256

int main()
{
    int fd[2];
    pid_t pid;
    int nbread;
    char buf[BUFSIZE];
    char *message = "Hello, I am process A\n";
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        close(fd[1]);
        if ((nbread = read(fd[0], buf, BUFSIZE)) == -1)
        {
            perror("read");
            exit(1);
        }
        printf("The read message is: %s\n", buf);
        close(fd[0]);
        exit(0);
    }
    close(fd[0]);
    write(fd[1], message, strlen(message) + 1);
    close(fd[1]);
    wait(NULL);
    return 0;
}