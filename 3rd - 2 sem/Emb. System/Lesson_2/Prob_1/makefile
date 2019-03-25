all: UDPEchoClient	UDPEchoServer

UDPEchoClient: UDPEchoClient.o
	gcc -g -o UDPEchoClient UDPEchoClient.o
UDPEchoServer: UDPEchoServer.o
	gcc -g -o UDPEchoServer UDPEchoServer.o

UDPEchoClient.o: UDPEchoClient.c prob_1.h
	gcc -g -c UDPEchoClient.c
UDPEchoServer.o: UDPEchoServer.c prob_1.h
	gcc -g -c UDPEchoServer.c

clean:
	rm UDPEchoClient UDPEchoServer *.o