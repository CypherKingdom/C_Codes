#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>

int server(int clientid)
{
    while (1)
    {
        int length;
        char *text;
        if (read(clientid, &length, sizeof(length)) == 0)
            return 0;
        text = malloc(length);
        read(clientid, text, length);
        printf("%s\n", text);
        if (!strcmp(text, "quit"))
            return 1;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid number of arguments, supply 2 arguments...");
        exit(1);
    }
    int socketid;
    struct sockaddr_un name;
    int quitmessage;
    socketid = socket(PF_LOCAL, SOCK_STREAM, 0);
    strcpy(name.sun_path, argv[1]);
    name.sun_family = AF_LOCAL;
    bind(socketid, &name, SUN_LEN(&name));
    listen(socketid, 5);
    do
    {
        int clientid;
        struct sockaddr_un clientname;
        clientid = accept(socketid, &clientname, SUN_LEN(&clientname));
        quitmessage = server(clientid);
        close(clientid);
    } while (!quitmessage);
    close(socketid);
    unlink(argv[1]);
    return 0;
}
