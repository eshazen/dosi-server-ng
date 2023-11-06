#ifndef DEBUG_LOG_HH
#define DEBUG_LOG_HH
//
// DebugLog.hh - very simple debug message handler
// slightly patterned after syslog
//
// Log message levels below, lower value is higher priority
// to log a message, e.g:
//   DebugLog::log( LOG_NOTICE, "something might be wrong");
//
// Set the level to WARNING and more serious:
//   DebugLog::setlogmask( LOG_UPTO( LOG_WARNING));
// (the default is to log everything)
//

#define  LOG_EMERG    0 //  system is unusable
#define  LOG_ALERT    1 //  action must be taken immediately
#define  LOG_CRIT     2 //  critical conditions
#define  LOG_ERR      3 //  error conditions
#define  LOG_WARNING  4 //  warning conditions
#define  LOG_NOTICE   5 //  normal, but significant, condition
#define  LOG_INFO     6 //  informational message
#define  LOG_DEBUG    7 //  debug-level message

#define LOG_MASK(c) (1<<c)
#define LOG_UPTO(n) DebugLog::LogUpto(n)

class DebugLog {

public:
  // default all messages enabled
  static void setlogmask( int m) { DebugLog::mask = m; };
  static void log( int priority, const char *format, ...);
  static unsigned mask;
  static int LogUpto( int lev);
};
#endif
