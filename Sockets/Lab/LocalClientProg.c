/*
    Write two programs: a client and a server, that communicate using  a  socket. The client
    sends a character string entered from the keyboard to the server via the socket created. The
    server reads the message received from the client and displays it on the screen. The server
    then sends back to the client the message "I got your message", which will be displayed by
    the client. Using a local socket in connected mode with up to 5 simultaneous connections. The
    socket name is taken as a parameter when the programs are run.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

void error(char *);

void main(int argc, char *argv[])
{
    int sockfd, servlen, n;
    struct sockaddr_un serv_addr;
    char buffer[82];
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[1]);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        error("Creating socket");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
        error("Connecting");
    printf("Please enter your message: ");
    bzero(buffer, 82);
    fgets(buffer, 80, stdin);
    write(sockfd, buffer, strlen(buffer));
    n = read(sockfd, buffer, 80);
    printf("The return message was\n");
    write(1, buffer, n);
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}