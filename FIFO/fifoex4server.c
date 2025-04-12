#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define SIZE 10

struct message
{
    int Nb;
    int a[SIZE];
};

int main()
{
    int fdread, fdwrite, nbRead, res = 0;
    const int max = sizeof(int);
    char buf[max];
    struct message mesg;
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
    nbRead = read(fdread, &mesg, sizeof(struct message));
    if (nbRead == -1)
    {
        perror("read");
        exit(1);
    }
    for (int i = 0; i < mesg.Nb; i++)
        res += mesg.a[i];
    sprintf(buf, "%d", res);
    write(fdwrite, buf, strlen(buf) + 1);
    close(fdread);
    close(fdwrite);
    unlink("/tmp/fifo1");
    return 0;
}