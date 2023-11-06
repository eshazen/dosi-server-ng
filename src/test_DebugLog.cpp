
#include "DebugLog.hh"

int main( int argc, char *argv[]) {

  DebugLog::log( LOG_ERR, "This is a an error\n");
  DebugLog::setlogmask( LOG_UPTO(LOG_WARNING));
  DebugLog::log( LOG_INFO, "This is an info\n");
  
}
