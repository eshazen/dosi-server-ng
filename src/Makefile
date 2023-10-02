
all: server

clean:
	rm -f *.o
	rm -f server

main.o: main.cpp TcpServer.hh

TcpServer.o: TcpServer.cpp TcpServer.hh

server: TcpServer.o main.o
	g++ -o server TcpServer.o main.o
