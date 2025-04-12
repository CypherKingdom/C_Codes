#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

void gethomepage(int socket)
{
    char buffer[10000];
    int nbRead;
    sprintf(buffer, "GET /\n");
    write(socket, buffer, strlen(buffer));
    while (nbRead = read(socket, buffer, 10000))
        write(STDOUT_FILENO, buffer, nbRead);
}

int main(int argc, char *argv[])
{
    int socketcl;
    struct sockaddr_in address;
    struct hostent *server;
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments..\nusage: %s hostname portnumber", argv[0]);
        exit(1);
    }
    socketcl = socket(PF_INET, SOCK_STREAM, 0);
    if (socketcl < 0)
    {
        perror("Error socket");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if (server = NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    bcopy(server->h_addr_list[0], &address.sin_addr.s_addr, server->h_length); //h_addr is deprecated I used h_addr_list
    if (connect(socketcl, &address, sizeof(address)) < 0)
    {
        perror("Connect");
        exit(1);
    }
    gethomepage(socketcl);
    return 0;
}