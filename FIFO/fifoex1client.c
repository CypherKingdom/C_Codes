// Client

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE 256

int main()
{
    mode_t mode;
    int fdwrite, fdread, nbRead;
    char message[BUFSIZE], buf[BUFSIZE];
    mode = 0600;
    if (mkfifo("fifo1", mode) == -1 && errno != EEXIST)
        fprintf(stderr, "can't create fifo1\n");
    fdwrite = open("./fifo1", O_WRONLY | O_NONBLOCK);
    if (fdwrite == -1)
    {
        perror("open1");
        exit(1);
    }
    fdread = open("./fifo2", O_RDONLY | O_NONBLOCK);
    if (fdread == -1)
    {
        perror("open1");
        exit(1);
    }
    sprintf(message, "Hello fd=%d", fdwrite);
    write(fdwrite, message, strlen(message) + 1);
    nbRead = read(fdread, buf, BUFSIZE);
    if (nbRead == -1)
    {
        perror("read");
        exit(1);
    }
    buf[strlen(buf)] = '\0';
    printf("ECHO:%s\n", buf);
    close(fdwrite);
    close(fdread);
    unlink("fifo1");
    unlink("fifo2");
    return 0;
}