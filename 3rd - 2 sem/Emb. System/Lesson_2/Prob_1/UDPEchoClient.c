#include "prob_1.h"

void
dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen, char ** argv);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2){
		fputs("usage: udpcli <IPaddress>", stdout);
        exit(-1);
    }

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

	dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr), argv);

	exit(0);
}

void
dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen, char ** argv)
{
	int	n;
	char	sendline[MAX_BUFF], recvline[MAX_BUFF + 1];

    for ( ; ; ){
        printf("Input: ");
        fgets(sendline, MAX_BUFF, fp);
        int l = strlen(sendline) - 1;
        sendline[l] = 0;

        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        printf("UDPEchoClient: sent %d byte(s) to %s\n", l, argv[1]);

		n = recvfrom(sockfd, recvline, MAX_BUFF, 0, NULL, NULL);

		recvline[n] = 0;	/* null terminate */
        printf("Receive: %s\n\n", recvline);
    }
}