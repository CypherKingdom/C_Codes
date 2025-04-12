/*
    Write a program that creates a ring of 3 processes connected by pipes.
    – The first process should ask the user for a string and then send it to the second
    process.
    – The second process should reverse the string and send it to the third process.
    – The third process should convert the string to uppercase and send it back to the
    first process.
    • When the first process gets the processed string, it should display it to the terminal.
    • When this is done, all 3 processes should terminate.
    • Here's an illustration of the process ring: prompt/display -> Reverse -> Uppercase -> prompt/display
    • Here's an example of the program in action:
    $ ring ...run the program.
    Please enter a string: ole
    Processed string is: ELO
    $ _
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/wait.h>
#define BUFSIZE 256

void reverseLine(char *, int);

int main()
{
    char buf1[BUFSIZE], buf2[BUFSIZE], buf3[BUFSIZE], buf4[BUFSIZE];
    int fd1[2], fd2[2], fd3[2], i, nbRead, nbRead1, pid1, pid2;
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
    if (pipe(fd3) == -1)
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
    { // Parent
        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid2 != 0)
        { // Parent
            close(fd1[0]);
            close(fd3[1]);
            close(fd2[0]);
            close(fd2[1]);
            while (nbRead = read(STDIN_FILENO, buf1, BUFSIZE))
            {
                write(fd1[1], buf1, nbRead);
                nbRead1 = read(fd3[0], buf4, BUFSIZE);
                if (nbRead1 == -1)
                {
                    perror("read");
                    exit(1);
                }
                write(STDOUT_FILENO, buf4, nbRead1);
            }
            // close descriptors;
            close(fd1[1]);
            close(fd3[0]);
            while (wait(NULL) > 0)
                ;
            exit(0);
        }
        else
        { // Child2
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            close(fd3[0]);
            while (nbRead = read(fd2[0], buf3, BUFSIZE))
            {
                for (i = 0; i < nbRead; i++)
                    buf3[i] = toupper(buf3[i]);
                write(fd3[1], buf3, nbRead);
            }
            close(fd3[1]);
            close(fd2[0]);
            exit(0);
        }
    }
    else
    { // child1
        close(fd3[0]);
        close(fd3[1]);
        close(fd1[1]);
        close(fd2[0]);
        while (nbRead = read(fd1[0], buf2, BUFSIZE))
        {
            reverseLine(buf2, nbRead - 1);
            write(fd2[1], buf2, nbRead);
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    return 0;
}

void reverseLine(char *buffer, int n)
{
    int i;
    char tmp;
    for (i = 0; i < n / 2; i++)
    {
        tmp = buffer[i];
        buffer[i] = buffer[n - i - 1];
        buffer[n - i - 1] = tmp;
    }
}