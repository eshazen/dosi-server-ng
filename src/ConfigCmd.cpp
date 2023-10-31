// ConfigCmd.cpp - class to contain one DOSI configuration command
//

#include <cstdio>
#include <algorithm>		// for min, max
#include <cstring>
#include "ConfigCmd.hh"

using namespace std;

//
// construct a new item with pointer to lambda or function to take action
// commands with only one string e.g. "sx"
ConfigCmd::ConfigCmd( ConfigCmdType_t t, const char *str, ConfFunc_t fun) {
  match_string = str;
  match_string2 = NULL;
  action = fun;
  type = t;
}

// commands with two strings, e.g. "pam c"
ConfigCmd::ConfigCmd( ConfigCmdType_t t, const char *str, const char *str2, ConfFunc_t fun) {
  match_string = str;
  match_string2 = str2;
  action = fun;
  type = t;
}

// destructor doesn't do anything at this time
ConfigCmd::~ConfigCmd() { };


//
// check if item matches against parsed command tokens in pv (count=n)
// return item type from enum ( CMD_ERROR, CMD_CONF, CMD_HW, CMD_GO)
//
int ConfigCmd::Match( const char **pv, int num_pv) {
  if( num_pv < 1) return CMD_ERROR;
  if( match_string2 == NULL) {	// single string to match
    if( !strncasecmp( pv[0], match_string, min(strlen(pv[0]),strlen(match_string))))
      return type;
    else
      return CMD_ERROR;
  } else {
    if( num_pv < 2) return CMD_ERROR;
    // check against two strings
    if( !strncasecmp( pv[0], match_string, min(strlen(pv[0]), strlen(match_string))) &&
	!strncasecmp( pv[1], match_string2, min(strlen(pv[1]),strlen(match_string2))))
      return type;
    else
      return CMD_ERROR;
  }      
}

static const char* types[] = {"Conf", "HW", "Go" };

//
// minimal funcction to print an item
//
void ConfigCmd::Print() {
  if( type >= 0 && type < (sizeof(types)/sizeof(types[0])))
    printf("Type=%s ", types[type]);
  else
    printf( "Type=%d ", type);
  printf( "Match string 1 = %s", match_string);
  if( match_string2)
    printf( " string 2 = %s", match_string2);
  printf("\n");
}


//
// access the command type
//
ConfigCmdType_t ConfigCmd::Type() {
  return type;
}
