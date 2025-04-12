#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Invalid number of arguments, supply 3 arguments...");
        exit(1);
    }
    int clientid, length;
    struct sockaddr_un name;
    clientid = socket(PF_LOCAL, SOCK_STREAM, 0);
    strcpy(name.sun_path, argv[1]);
    name.sun_family = AF_LOCAL;
    connect(clientid, &name, SUN_LEN(&name));
    length = strlen(argv[2]) + 1;
    write(clientid, &length, sizeof(length));
    write(clientid, argv[2], length);
    close(clientid);
    return 0;
}