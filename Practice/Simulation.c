#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s FILEIN FILEOUT\n", argv[0]);
        exit(1);
    }

    int pipefd[2], pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid != 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp("grep", "grep", "Fanar", argv[1], NULL);
        perror("grep");
        exit(1);
    }
    else
    {
        close(pipefd[1]);
        int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fd_out == -1)
        {
            perror("Error opening FILEOUT");
            exit(1);
        }
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
        execlp("cat", "cat", NULL);
        perror("cat");
        exit(1);
    }
    return 0;
}