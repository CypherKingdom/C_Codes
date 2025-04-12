#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd1[2], fd2[2], pid1, pid2;
    if (pipe(fd1) == -1 || pipe(fd2) == -1)
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
    if (pid1 != 0)
    {
        close(fd1[0]);
        dup2(fd1[1], STDOUT_FILENO);
        close(fd1[1]);
        execlp("ps", "ps", "aux", NULL);
        perror("ps");
        exit(1);
    }
    else
    {
        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(1);
        }
        if (pid2 != 0)
        {
            close(fd1[1]);
            close(fd2[0]);
            dup2(fd1[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);
            close(fd1[0]);
            close(fd2[1]);
            execlp("grep", "grep", "root", NULL);
            perror("grep");
            exit(1);
        }
        else
        {
            close(fd2[1]);
            dup2(fd2[0], STDIN_FILENO);
            close(fd2[0]);
            execlp("wc", "wc", "-l", NULL);
            perror("wc");
            exit(1);
        }
    }
    wait(NULL);
    wait(NULL);
    return 0;
}