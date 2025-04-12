/*
    Write a C program that is equivalent to the shell command:
    ps aux | grep root | wc -l
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int fd1[2], fd2[2], pid1, pid2;
    pipe(fd1);
    if (pipe(fd1) == -1)
    {
        perror("pipe");
        exit(1);
    }
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid1 != 0)
    {
        close(fd1[0]);
        dup2(fd1[1], 1);
        execlp("ps", "ps", "aux", NULL);
        perror("ps");
        exit(1);
    }
    else
    {
        close(fd1[1]);
        pipe(fd2);
        if (pipe(fd2) == -1)
        {
            perror("pipe");
            exit(1);
        }
        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid2 != 0)
        {
            close(fd2[0]);
            dup2(fd1[0], 0);
            dup2(fd2[1], 1);
            execlp("grep", "grep", "root", NULL);
            perror("grep");
            exit(1);
        }
        else
        {
            close(fd2[1]);
            dup2(fd2[0], 0);
            execlp("wc", "wc", "-l", NULL);
            perror("wc");
            exit(1);
        }
    }
    return 0;
}