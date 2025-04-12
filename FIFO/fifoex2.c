#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE 256

int main()
{
    mode_t mode;
    int fdwrite, fdread;
    char message[BUFSIZE], buf[BUFSIZE];
    mode = 0600;
    unlink("/tmp/fifo1");
    unlink("/tmp/fifo2");
    if (mkfifo("/tmp/fifo1", mode) == -1 && errno != EEXIST)
        fprintf(stderr, "can't create fifo1\n");
    if (mkfifo("/tmp/fifo2", 0600) == -1 && errno != EEXIST)
        fprintf(stderr, "can't create fifo2\n");
    if (fork() == 0)
    {
        fdread = open("/tmp/fifo1", O_RDONLY | O_NONBLOCK);
        if (fdread == -1)
        {
            perror("open1");
            exit(1);
        }
        fdwrite = open("/tmp/fifo2", O_WRONLY | O_NONBLOCK);
        if (fdwrite == -1)
        {
            perror("open2");
            exit(1);
        }
        read(fdread, buf, BUFSIZE);
        if (read == -1)
        {
            perror("read");
            exit(1);
        }
        buf[strlen(buf)] = '\0';
        sprintf(message, "%d:%s", getpid(), buf);
        write(fdwrite, message, strlen(message) + 1);
        exit(0);
    }
    fdwrite = open("/tmp/fifo1", O_WRONLY | O_NONBLOCK);
    if (fdwrite == -1)
    {
        perror("open1");
        exit(1);
    }
    fdread = open("/tmp/fifo2", O_RDONLY | O_NONBLOCK);
    if (fdread == -1)
    {
        perror("open1");
        exit(1);
    }
    sprintf(message, "Hello fd=%d", fdwrite);
    write(fdwrite, message, strlen(message) + 1);
    read(fdread, buf, BUFSIZE);
    if (read == -1)
    {
        perror("read");
        exit(1);
    }
    buf[strlen(buf)] = '\0';
    printf("Echo:%s\n", buf);
    close(fdwrite);
    close(fdread);
    return 0;
}