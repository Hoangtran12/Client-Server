#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

int timed = 0; // Variable to store the alarm duration

// Signal handler
void alarm_handler(int signum) {
    printf("** alarm interrupted after no activity, to be terminated **\n");
    kill(0, SIGTERM);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <time>\n", argv[0]);
        return 1;
    }

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    // Set the alarm duration from the command-line argument
    timed = atoi(argv[2]);

    // Print messages at the beginning
    printf("** the server uses port# %s to listen with %s for alarm **\n", argv[1], argv[2]);
    printf("** the server will be terminated after %s seconds of inactivity **\n", argv[2]);
    system("date; hostname; whoami; ps; ls -l");

    signal(SIGALRM, alarm_handler); // Set a signal handler for SIGALRM

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    alarm(0); // Reset the alarm (no alarm)

    while (1) {
        // Set the alarm for the specified duration
        alarm(timed);

        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        // netstat cmd
        char netstat_command[100];
        sprintf(netstat_command, "netstat -t -n | grep '127.0.0.1:%s'", argv[1]);
        system(netstat_command);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        alarm(0); // Reset the alarm
    }

    // Print a message at the end
    printf("** the server ends **\n");
    system("date; hostname; whoami; ps; ls -l");

    return 0;
}
