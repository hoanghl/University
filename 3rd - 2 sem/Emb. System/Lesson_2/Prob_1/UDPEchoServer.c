#include "prob_1.h"

void dg_echo(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen);

int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
        perror("bind");
    
    dg_echo(sockfd, &cliaddr, sizeof(servaddr));
    return 0;
}

void dg_echo(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen){
    int         n;
    socklen_t   len;
    char    mesg[MAX_BUFF];
    struct sockaddr *pcliaddr = (struct sockaddr *)cliaddr;

    for ( ; ; ) {
        len = clilen;
        fputs("UDPEchoServer: Server is waiting for packets ...\n", stdout);
        n = recvfrom(sockfd, mesg, MAX_BUFF, 0, pcliaddr, &len);
        mesg[n] = 0;
    
        char cliadd_str[16];
        inet_ntop(AF_INET, &cliaddr->sin_addr, cliadd_str, sizeof(*cliaddr));
        printf("UDPEchoServer: got packet from %s\n", cliadd_str);
        printf("UDPEchoServer: packet is %d byte(s) long\n", n);
        printf("UDPEchoServer: packet contains '%s'\n", mesg);

        printf("UDPEchoServer: send packet back to client\n\n");
        sendto(sockfd, mesg, n, 0, pcliaddr, sizeof(*pcliaddr));
    }
}