/*
    Write 2 programs in C, one is a client and the other is a server.
    Both communicate using named pipes.
    The client sends many integers to the server,
    and the server calculates their sum and returns the result to the client.
*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE 256

int main()
{
    int fdwrite, fdread, i, n, res, N, Nw;
    const int max = sizeof(int);
    char buf[BUFSIZE];
    if ((mkfifo("/tmp/fifo2", 0600)) == -1 && errno != EEXIST)
        fprintf(stderr, "can't create fifo2\n");
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
    printf("Enter the number of integers:\n");
    scanf("%d", &N);
    sprintf(buf, "%d", N);
    write(fdwrite, buf, max);
    printf("Enter %d integers to add:\n", N);
    for (i = 0; i < N; i++)
    {
        scanf("%d", &n);
        sprintf(buf, "%d", n);
        Nw = write(fdwrite, buf, max);
    }
    // read result
    read(fdread, buf, strlen(buf) + 1);
    if (read == -1)
    {
        perror("read");
        exit(1);
    }
    res = atoi(buf);
    printf("The received result is : %d\n", res);
    close(fdread);
    close(fdwrite);
    unlink("/tmp/fifo2");
    return 0;
}