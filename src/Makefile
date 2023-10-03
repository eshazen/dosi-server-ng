
all: server

clean:
	rm -f *.o
	rm -f server

main.o: main.cpp TcpServer.hh
TcpServer.o: TcpServer.cpp TcpServer.hh
server.o: server.cpp
interactive_test: TcpServer.o main.o
	g++ -o $@ $^
server: TcpServer.o server.o
	g++ -o $@ $^
