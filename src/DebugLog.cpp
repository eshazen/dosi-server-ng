
#include "DebugLog.hh"
#include <cstdio>
#include <cstdarg>

unsigned DebugLog::mask = 0xff;

int DebugLog::LOG_UPTO( int lev) { 
  int m = 0;
  for(int i=0;i<=lev;i++) 
    m|=LOG_MASK(i);
  return m;
 };

void DebugLog::log( int priority, const char *format, ...) {
  va_list argptr;
  if( LOG_MASK(priority) & mask) {
    va_start( argptr, format);
    vfprintf( stderr, format, argptr);
    va_end(argptr);
  }
}
