EXECS = test_TcpServer test_config test_DebugLog dosi_server

all: ${EXECS}

clean:
	rm -f *.o
	rm -f ${EXECS}

dosi_server.o: dosi_server.cpp DebugLog.hh TcpServer.hh
TcpServer.o: TcpServer.cpp TcpServer.hh
test_TcpServer.o: test_TcpServer.cpp TcpServer.hh
test_DebugLog.o: test_DebugLog.cpp DebugLog.hh
test_config.o: test_config.cpp DosiConfig.hh ConfigCmdList.hh ConfigCmd.hh ParseString.hh DOSI_HW.hh
DosiConfig.o: DosiConfig.cpp DosiConfig.hh
ConfigCmd.o: ConfigCmd.cpp ConfigCmd.hh DosiConfig.hh DOSI_HW.hh
ConfigCmdList.o: ConfigCmdList.cpp ConfigCmdList.hh ConfigCmd.hh DosiConfig.hh ParseString.hh
ParseString.o: ParseString.cpp ParseString.hh
DebugLog.o: DebugLog.cpp DebugLog.hh

test_TcpServer: TcpServer.o test_TcpServer.o DebugLog.o
	g++ -o $@ $^

test_config: ConfigCmd.o ConfigCmdList.o DosiConfig.o test_config.o ParseString.o DOSI_HW.o DebugLog.o
	g++ -o $@ $^

test_DebugLog: test_DebugLog.o DebugLog.o
	g++ -o $@ $^

dosi_server: dosi_server.o TcpServer.o DebugLog.o ConfigCmd.o ConfigCmdList.o DosiConfig.o \
             ParseString.o DOSI_HW.o
	g++ -o $@ $^
