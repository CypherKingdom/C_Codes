/*
    Write 2 programs in C, one is a client and the other is a server.
    Both communicate using named pipes.
    The client sends many integers to the server,
    and the server calculates their sum and returns the result to the client.
*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fdread, fdwrite, res = 0, nbRead;
    const int max = sizeof(int);
    char buf[max];
    if ((mkfifo("/tmp/fifo1", 0600)) == -1 && errno != EEXIST)
        fprintf(stderr, "can't create fifo1\n");
    printf("Server is waiting the client to open...\n");
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
    nbRead = read(fdread, buf, max);
    if (nbRead == -1)
    {
        perror("read");
        exit(1);
    }
    for (int i = 0; i < atoi(buf); i++)
    {
        nbRead = read(fdread, buf, max);
        if (nbRead == -1)
        {
            perror("read");
            exit(1);
        }
        res += atoi(buf);
    }
    sprintf(buf, "%d", res);
    write(fdwrite, buf, max);
    close(fdread);
    close(fdwrite);
    unlink("/tmp/fifo1");
    return 0;
}