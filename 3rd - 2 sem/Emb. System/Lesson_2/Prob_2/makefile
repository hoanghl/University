no_multi: TCPEchoClient	TCPEchoServer
multi: TCPEchoClient	TCPEchoServer_multi

TCPEchoServer_multi: TCPEchoServer_multi.o
	gcc -g -o TCPEchoServer_multi TCPEchoServer_multi.o
TCPEchoClient: TCPEchoClient.o
	gcc -g -o TCPEchoClient TCPEchoClient.o
TCPEchoServer: TCPEchoServer.o
	gcc -g -o TCPEchoServer TCPEchoServer.o

TCPEchoClient.o: TCPEchoClient.c
	gcc -g -c TCPEchoClient.c
TCPEchoServer.o: TCPEchoServer.c
	gcc -g -c TCPEchoServer.c
TCPEchoServer_multi.o: TCPEchoServer_multi.c
	gcc -g -c TCPEchoServer_multi.c
clean:
	rm TCPEchoClient TCPEchoServer TCPEchoServer_multi *.o

