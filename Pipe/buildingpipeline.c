/*
    Building pipeline using unnamed pipe
    – connect the standard output of one process to the standard input of
    another.
    – E.g., who | wc ; ls | sort

    – It assumes that neither program is invoked with options, and that the
    names of the programs are listed on the command line.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
    int fd[2], pid;
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
    else if (pid != 0) // Parent, writer
    {
        close(fd[0]);                   // Close unused end
        dup2(fd[1], 1);                 // Duplicate used end to stdout
        close(fd[1]);                   // Close original used end
        execlp(argv[1], argv[1], NULL); // Execute writer program
        perror("connect");              // Should never execute
    }
    else // Child, reader
    {
        close(fd[1]);                   // Close unused end
        dup2(fd[0], 0);                 // Duplicate used end to stdin
        close(fd[0]);                   // Close original used end
        execlp(argv[2], argv[2], NULL); // Execute reader program
        perror("connect");              // Should never execute
    }
}