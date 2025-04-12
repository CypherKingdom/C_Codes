/*
    Another solution using structures
    [we can use structures also with pipes]
*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE 256
#define SIZE 10

struct message
{
    int Nb;
    int a[SIZE];
};

int main()
{
    int fdwrite, fdread, nbRead;
    char buf[BUFSIZE];
    struct message mesg;
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
    printf("Enter the number of integers: \n");
    scanf("%d", &mesg.Nb);
    printf("Enter %d integers to add\n", mesg.Nb);
    for (int i = 0; i < mesg.Nb; i++)
        scanf("%d", &mesg.a[i]);
    write(fdwrite, &mesg, sizeof(mesg));
    nbRead = read(fdread, buf, BUFSIZE);
    if (nbRead == -1)
    {
        perror("read");
        exit(1);
    }
    printf("The received result is : %d\n", atoi(buf));
    close(fdread);
    close(fdwrite);
    unlink("/tmp/fifo2");
    return 0;
}