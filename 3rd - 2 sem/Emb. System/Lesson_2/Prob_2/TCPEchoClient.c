#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT    54950

#define MAX_BUFF    100

int main(int argc, char ** argv){
    int                 sockfd;
    struct sockaddr_in  seraddr;

    if (argc != 2){
        printf("usage: client <server_ip>");
        return -1;
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        perror("socket");

    memset(&seraddr, 0, sizeof(struct sockaddr_in));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
        perror("connect");

    for ( ; ; ) {
        printf("Input: ");
        char msg[MAX_BUFF];
        fgets(msg, MAX_BUFF, stdin);
        int msg_len = strlen(msg) - 1;
        msg[msg_len] = 0;

        printf("TCPEchoClient: send %d byte(s) to %s\n", msg_len, argv[1]);
        if (send(sockfd, msg, msg_len, 0) == -1)
            perror("send");


        char * buff = malloc(MAX_BUFF);
        if ( (msg_len = recv(sockfd, buff, MAX_BUFF - 1, 0)) == -1)
            perror("recv");
        buff[msg_len] = 0;
        printf("Receive: %s\n", buff);

        free(buff);
    }
    return 0;
}