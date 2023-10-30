
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

test_config.o: test_config.cpp DosiConfig.hh ConfigCmdList.hh ConfigCmd.hh ParseString.hh DOSI_HW.hh
DosiConfig.o: DosiConfig.cpp DosiConfig.hh
ConfigCmd.o: ConfigCmd.cpp ConfigCmd.hh DosiConfig.hh DOSI_HW.hh
ConfigCmdList.o: ConfigCmdList.cpp ConfigCmdList.hh ConfigCmd.hh DosiConfig.hh ParseString.hh
ParseString.o: ParseString.cpp ParseString.hh

test_config: ConfigCmd.o ConfigCmdList.o DosiConfig.o test_config.o ParseString.o
	g++ -o $@ $^
