/*
    Create 2 programs: one a server and the other one a client. A client sends its
    pid and the pathname of a file to the server. The server can accept the request of many
    clients. It opens the requested file and sends its content to the correspondent client. If
    the server cannot open the file, it sends an error message to the client.

    See Figure.png!!!
*/

#include <unistd.h>
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
    int fdread, fdwrite, pid;
    struct Message mesg;
    if (argc != 2)
    {
        fprintf(stderr, "arg error\n");
        exit(1);
    }
    strcpy(filename, argv[1]);
    sprintf(fifoname, "/tmp/fifo.%d", getpid());
    if (mkfifo(fifoname, 0600) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(1);
    }
    fdwrite = open("/tmp/fifo.serv", O_WRONLY | O_NONBLOCK);
    if (fdwrite == -1)
    {
        perror("open");
        exit(1);
    }
    printf("My fifoname is: %s\n", fifoname);
    mesg.pid = getpid();
    strcpy(mesg.filename, filename);
    write(fdwrite, &mesg, sizeof(mesg));
    fdread = open(fifoname, O_RDONLY | O_NONBLOCK);
    if (fdread == -1)
    {
        perror("open");
        exit(1);
    }
    /*
    write(fdwrite,&pid,sizeof(pid));
    write(fdwrite,filename,strlen(filename)+1));
    */
    Copying(fdread, STDOUT_FILENO, buf);
    fprintf(stderr, "Ending Connection\n");
    close(fdwrite);
    close(fdread);
    unlink(fifoname);
    return 0;
}

void Copying(int fdin, int fdout, char *buf)
{
    int nbRead;
    while (nbRead = read(fdin, buf, BUFSIZE))
        write(fdout, buf, nbRead);
}