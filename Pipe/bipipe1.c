#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define BUFSIZE 256

int main()
{
    int fd1[2], fd2[2], Nr1, Nr2;
    pid_t pid;
    char buf1[BUFSIZE], buf2[BUFSIZE];
    char *s1 = "hello, I am the process A\n";
    char *s2 = "hello, I am the process B\n";
    if (pipe(fd1) == -1)
    {
        perror("pipe");
        exit(1);
    }
    pipe(fd2);
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
        write(fd2[1], s2, strlen(s2) + 1);
        Nr2 = read(fd1[0], buf2, BUFSIZE);
        if (Nr2 == -1)
        {
            perror("read");
            exit(1);
        }
        printf("The string read by the child: %s\n", buf2); /* write(STDOUT_FILENO,buf2,Nr2);*/
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    close(fd1[0]);
    close(fd2[1]);
    /*printf("Enter the string to be read:\n"); //Enter from keyboard
    int nb=read(STDIN_FILENO,buf1,BUFSIZE) ;
    buf1[nb]='\0';
    write (fd1[1], buf1, nb+1); */
    write(fd1[1], s1, strlen(s1) + 1);
    Nr1 = read(fd2[0], buf1, BUFSIZE);
    if (Nr1 == -1)
    {
        perror("read");
        exit(1);
    }
    printf("The string read by the parent: %s\n", buf1);
    close(fd1[1]);
    close(fd2[0]);
    wait(NULL);
    return 0;
}