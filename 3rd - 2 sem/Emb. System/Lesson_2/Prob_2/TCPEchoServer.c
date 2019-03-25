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


int main(int argc, char** argv){
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("listen");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        perror("bind");
    
    if (listen(listenfd, 5) < 0)
        perror("listen");

    for ( ; ; ) {
        printf("TCPEchoClient: Server is waiting...\n");
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        char addr_tr[16];        
        inet_ntop(AF_INET, &cliaddr.sin_addr, addr_tr, sizeof(addr_tr));
        printf("TCPEchoClient: got request from %s\n", addr_tr);
        char * buff = malloc(MAX_BUFF);
        memset(buff, 0, MAX_BUFF);
        int l = recv(connfd, buff, MAX_BUFF, 0);
        printf("TCPEchoClient: packet is %d byte(s) long\n", l);
        printf("TCPEchoClient: packet contains '%s'\n", buff);
        
        printf("TCPEchoClient: send packet back to client\n");
        if (send(connfd, buff, l, 0) < 0)
            perror("send");        
    }
}