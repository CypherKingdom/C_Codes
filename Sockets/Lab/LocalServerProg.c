#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

void error(char *);

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, servlen, clilen, n;
    struct sockaddr_un cli_addr, serv_addr;
    char buf[80];
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        error("creating socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[1]);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
        error("binding socket");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("accepting");
    n = read(newsockfd, buf, 80);
    printf("A connection has been established\n");
    write(1, buf, n);
    write(newsockfd, "I got your message\n", 19);
    close(newsockfd);
    close(sockfd);
    unlink(argv[1]);
    return 0;
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}