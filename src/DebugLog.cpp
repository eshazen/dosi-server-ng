// DebugLog.cpp - very simple debug message logging library
//
#include "DebugLog.hh"
#include <cstdio>
#include <cstdarg>

unsigned DebugLog::mask = 0xff;

//
// return a mask with bits set up to the specified one
//
int DebugLog::LOG_UPTO( int lev) { 
  int m = 0;
  for(int i=0;i<=lev;i++) 
    m|=LOG_MASK(i);
  return m;
 };

//
// log a message if mask allows 'priority' level messages
// use printf-like syntax for messages
//
void DebugLog::log( int priority, const char *format, ...) {
  va_list argptr;
  if( LOG_MASK(priority) & mask) {
    va_start( argptr, format);
    vfprintf( stderr, format, argptr);
    va_end(argptr);
  }
}
