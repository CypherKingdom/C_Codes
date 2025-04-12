/*
    Write a C program that contains a producer process (reads from the stdin and
    write to a pipe) and a consumer process that makes capital letter the small letter (reads
    from the pipe and writes to the stdout).
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#define BUFSIZE 256

int main()
{
    int fd[2];
    int pid, nbRead;
    char c;
    char buf1[BUFSIZE], buf2[BUFSIZE];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid != 0)
    {
        close(fd[0]);
        while (nbRead = read(STDIN_FILENO, buf1, BUFSIZE))
            write(fd[1], buf1, nbRead);
        close(fd[1]);
        wait(NULL);
    }
    else
    {
        close(fd[1]);
        while (nbRead = read(fd[0], &c, 1))
        {
            c = toupper(c);
            write(STDOUT_FILENO, &c, nbRead);
        }
        close(fd[0]);
        exit(0);
    }
    return 0;
}