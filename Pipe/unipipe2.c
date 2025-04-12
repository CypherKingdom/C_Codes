#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFSIZE 256

void Copying(int, int, char *);

int main()
{
    int fd[2];
    pid_t pid;
    char buf1[BUFSIZE], buf2[BUFSIZE];
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
        Copying(fd[0], STDOUT_FILENO, buf2);
        close(fd[0]);
        exit(0);
    }
    close(fd[0]);
    printf("Write your message: ");
    Copying(STDIN_FILENO, fd[1], buf1);
    close(fd[1]);
    return 0;
}

void Copying(int in, int out, char *buf)
{
    int nbRead;
    while (nbRead = read(in, buf, BUFSIZE))
        write(out, buf, nbRead);
}