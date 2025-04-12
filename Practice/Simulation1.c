#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s FILEIN FILEOUT\n", argv[0]);
        exit(1);
    }
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0660);
    if (fd_out == -1)
    {
        perror("Error opening FILEOUT");
        exit(1);
    }
    if (dup2(fd_out, STDOUT_FILENO) == -1)
    {
        perror("dup2 failed");
        close(fd_out);
        exit(1);
    }
    execlp("grep", "grep", "Fanar", argv[1], NULL);
    close(fd_out);
    return 0;
}