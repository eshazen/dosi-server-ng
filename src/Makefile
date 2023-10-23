
all: server test_config

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

test_config.o: test_config.cpp ConfigParser.hh DosiConfig.hh
ConfigParser.o: ConfigParser.cpp ConfigParser.hh
DosiConfig.o: DosiConfig.cpp DosiConfig.hh

test_config: ConfigParser.o DosiConfig.o test_config.o
	g++ -o $@ $^
