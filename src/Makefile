
all: server test_config test_DebugLog interactive_test dosi_server

clean:
	rm -f *.o
	rm -f server

dosi_server.o: dosi_server.cpp DebugLog.hh TcpServer.hh
interactive_test.o: interactive_test.cpp TcpServer.hh
TcpServer.o: TcpServer.cpp TcpServer.hh
server.o: server.cpp
interactive_test: TcpServer.o interactive_test.o DebugLog.o
	g++ -o $@ $^
server: TcpServer.o server.o DebugLog.o
	g++ -o $@ $^

test_DebugLog.o: test_DebugLog.cpp DebugLog.hh
test_config.o: test_config.cpp DosiConfig.hh ConfigCmdList.hh ConfigCmd.hh ParseString.hh DOSI_HW.hh
DosiConfig.o: DosiConfig.cpp DosiConfig.hh
ConfigCmd.o: ConfigCmd.cpp ConfigCmd.hh DosiConfig.hh DOSI_HW.hh
ConfigCmdList.o: ConfigCmdList.cpp ConfigCmdList.hh ConfigCmd.hh DosiConfig.hh ParseString.hh
ParseString.o: ParseString.cpp ParseString.hh
DebugLog.o: DebugLog.cpp DebugLog.hh

test_config: ConfigCmd.o ConfigCmdList.o DosiConfig.o test_config.o ParseString.o DOSI_HW.o DebugLog.o
	g++ -o $@ $^

test_DebugLog: test_DebugLog.o DebugLog.o
	g++ -o $@ $^

dosi_server: dosi_server.o TcpServer.o DebugLog.o ConfigCmd.o ConfigCmdList.o DosiConfig.o \
             ParseString.o DOSI_HW.o
	g++ -o $@ $^
