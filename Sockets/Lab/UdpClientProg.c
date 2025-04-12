/*
    Write two programs: a client and a server, that communicate using  a  socket. The client
    sends a character string entered from the keyboard to the server via the socket created. The
    server reads the message received from the client and displays it on the screen. The server
    then sends back to the client the message "I got your message", which will be displayed by
    the client. Using remote sockets in non‐connected mode.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

void error(char *);

int main(int argc, char *argv[])
{
    int sock, length, n;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];

    if (argc != 3)
    {
        printf("Usage: server port\n");
        exit(1);
    }
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        error("socket");
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0)
        error("Unknown host");
    bcopy((char *)hp->h_addr_list[0], (char *)&server.sin_addr, hp->h_length); // USe h_addr_list[0] instead of h_addr
    server.sin_port = htons(atoi(argv[2]));
    length = sizeof(struct sockaddr_in);
    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = sendto(sock, buffer, strlen(buffer), 0, &server, length);
    if (n < 0)
        error("Sendto");
    n = recvfrom(sock, buffer, 256, 0, &from, &length);
    if (n < 0)
        error("recvfrom");
    write(1, "Got an ack: ", 12);
    write(1, buffer, n);
    close(sock);
    return 0;
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}