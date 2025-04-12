#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#define BUFSIZE 256

struct Message
{
    int pid;
    char filename[20];
};

void Copying(int, int, char *);

int main(int argc, char *argv[])
{
    char buf[BUFSIZE], fifoname[20], filename[20];
    int fdread, fdwrite, pid, fdin, nbRead;
    struct Message mesg;
    char *ErrorMesg = "Can't open file\n";
    if (mkfifo("/tmp/fifo.serv", 0600) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(1);
    }
    // open fifo server in Read mode
    fdread = open("/tmp/fifo.serv", O_RDONLY | O_NONBLOCK);
    if (fdread == -1)
    {
        perror("open");
        exit(1);
    }
    while (1)
    {
        nbRead = read(fdread, &mesg, sizeof(mesg));
        if (nbRead == -1)
        {
            perror("read");
            exit(1);
        }
        printf("nbRead=%d---\n", nbRead);
        if (nbRead == 0)
        {
            sleep(1);
            continue;
        }
        pid = mesg.pid;
        strcpy(filename, mesg.filename);
        sprintf(fifoname, "/tmp/fifo.%d", pid);
        printf("Opening %s...\n", fifoname);
        // open fifo client in Write mode
        fdwrite = open(fifoname, O_WRONLY | O_NONBLOCK);
        if (fdwrite == -1)
        {
            perror("open");
            exit(1);
        }
        printf("Opening %s...\n", filename);
        // open requested file name in Read mode
        fdin = open(filename, O_RDONLY | O_NONBLOCK);
        if (fdin == -1)
            write(fdwrite, ErrorMesg, strlen(ErrorMesg) + 1);
        else
            Copying(fdin, fdwrite, buf);
        close(fdwrite); // very important to end client
    };
    close(fdread);
    unlink("/tmp/fifo.serv");
    return 0;
}

void Copying(int fdin, int fdout, char *buf)
{
    int nbRead;
    while (nbRead = read(fdin, buf, BUFSIZE))
        write(fdout, buf, nbRead);
}