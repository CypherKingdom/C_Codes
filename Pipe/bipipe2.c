#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define BUFSIZE 256

void Copying(int, int, char *);

int main()
{
    int fd1[2], fd2[2], fd, len, nbRead, pid;
    char buf1[BUFSIZE], buf2[BUFSIZE];
    if (pipe(fd1) == -1)
    {
        perror("pipe");
        exit(1);
    }
    if (pipe(fd2) == -1)
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
    else if (pid == 0)
    {
        close(fd1[1]);
        close(fd2[0]);
        char *message = "can't open file\n";
        // read pathname from IPC channel
        if ((nbRead = read(fd1[0], buf2, BUFSIZE)) == 0)
            fprintf(stderr, "EOF while reading pathname");
        buf2[nbRead] = '\0'; // null terminate pathname
        if ((fd = open(buf2, O_RDONLY | O_NONBLOCK)) < 0)
        {
            // error: must tell client
            write(fd2[1], message, strlen(message));
        }
        else
        {
            // open succeeded: copy file to IPC channel
            Copying(fd, fd2[1], buf2);
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    close(fd1[0]);
    close(fd2[1]);
    printf("Enter the pathname:\n");
    // read pathname
    nbRead = read(STDIN_FILENO, buf1, BUFSIZE);
    if (nbRead == -1)
    {
        perror("read");
        exit(1);
    }
    buf1[nbRead - 1] = '\0';
    len = strlen(buf1); // to use strlen we need the \0 write pathname to IPC channel
    write(fd1[1], buf1, len); // we could send the \0, i.elen+1...
    // waiting reply, read it and write it to the screen 
    Copying(fd2[0], STDOUT_FILENO, buf1);
    close(fd1[1]);
    close(fd2[0]);
    wait(NULL);
    return 0;
}

void Copying(int in, int out, char *buf)
{
    int nbRead;
    while (nbRead = read(in, buf, BUFSIZE))
        write(out, buf, nbRead);
}