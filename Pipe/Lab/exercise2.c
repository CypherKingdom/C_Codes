/*
    Write a C program that creates 2 processes A and B that can communicate via
    a pipe. The process "A" open a file given by argument to the program and transfers the
    content of the file to process B via the pipe. The process "B" write the content of the pipe
    to a second file given also by argument. Verify if the first file have the default permissions 0644.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#define BUFSIZE 256

void Copying(int, int, char *);

int main(int argc, char *argv[])
{
    char buf1[BUFSIZE], buf2[BUFSIZE];
    int childpid, fd1, fd2, fd[2], i;
    char *fsrc, *fdest;
    struct stat infofile1;
    if (argc != 3)
    {
        fprintf(stderr, "Usage error");
        exit(1);
    }
    fsrc = argv[1];
    fdest = argv[2];
    fd1 = open(fsrc, O_RDONLY | O_NONBLOCK);
    if (fd1 == -1)
    {
        perror("open:");
        exit(1);
    }
    fstat(fd1, &infofile1);
    printf("mode=%d \n", infofile1.st_mode);
    if (infofile1.st_mode == 0100644)
        printf("Default permissions\n");
    if (pipe(fd) == -1)
    {
        perror("Failed to create the pipe");
        exit(1);
    }
    childpid = fork();
    if (childpid == -1)
    {
        perror("Failed to fork");
        exit(1);
    }
    if (childpid != 0)
    {
        //------ parent code ------//
        close(fd[0]);
        Copying(fd1, fd[1], buf1);
        close(fd[1]);
        close(fd1);
        if (wait(NULL))
            fprintf(stderr, "--child finished--");
        fprintf(stderr, "--Bye in parent--");
    }
    else
    {
        //--------child code------//
        close(fd[1]);
        fd2 = open(fdest, O_CREAT | O_WRONLY | O_NONBLOCK, 0600);
        if (fd2 == -1)
        {
            perror("open:");
            exit(1);
        }
        Copying(fd[0], fd2, buf2);
        close(fd2);
        close(fd[0]);
        fprintf(stderr, "--Bye in child--");
        exit(0);
    }
    return 0;
}

void Copying(int in, int out, char *buf)
{
    int nbRead;
    while (nbRead = read(in, buf, BUFSIZE))
        write(out, buf, nbRead);
}