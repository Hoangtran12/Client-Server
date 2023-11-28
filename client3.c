#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];  // IP address provided as the first command-line argument
    int port = atoi(argv[2]);  // Port number provided as the second command-line argument

    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error: Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    argv[1] = ip;

    // Print messages at the beginning
    printf("** the client is to connect to the server of IP# %s and Port# %d **\n", ip, port);
    printf("** the client will be terminated **\n");
    system("date; hostname; whoami; ps; ls -l");

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\n inet_pton error occurred\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error: Connect Failed \n");
        return 1;
    }

    printf("\n timeClient: connected to timeServer. \n");
    system("ps");

    // Print a message at the end
    printf("** the client ends **\n");
    system("date; hostname; whoami; ps; ls -l");

    return 0;
}
