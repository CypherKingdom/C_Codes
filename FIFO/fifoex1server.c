// Server

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE 256

int main()
{
    int fdread, fdwrite, nbRead;
    char buf[BUFSIZE], message[BUFSIZE];
    if (mkfifo("fifo2", 0600) == -1 && errno != EEXIST)
        fprintf(stderr, "can't create fifo2\n");
    fdread = open("./fifo1", O_RDONLY | O_NONBLOCK); // opening
    if (fdread == -1)
    {
        perror("open1");
        exit(1);
    }
    fdwrite = open("./fifo2", O_WRONLY | O_NONBLOCK);
    if (fdwrite == -1)
    {
        perror("open2");
        exit(1);
    }
    nbRead = read(fdread, buf, BUFSIZE);
    if (nbRead == -1)
    {
        perror("read");
        exit(1);
    }
    buf[strlen(buf)] = '\0';                  // we can remove it
    sprintf(message, "%d:%s", getpid(), buf); // it adds by default a \0 at the end
    // we can treat the msg here, like capitalize etc... write(fdwrite,message,strlen(message)+1);
    close(fdread);
    close(fdwrite);
    return 0;
}