#include <cstdio>
#include <algorithm>
#include <cstring>
#include "ConfigCmd.hh"

using namespace std;

//
// construct a new item with pointer to lambda or function to take action
//
ConfigCmd::ConfigCmd( ConfigCmdType_t t, const char *str, ConfFunc_t fun) {
  match_string = str;
  match_string2 = NULL;
  action = fun;
  type = t;
}

ConfigCmd::ConfigCmd( ConfigCmdType_t t, const char *str, const char *str2, ConfFunc_t fun) {
  match_string = str;
  match_string2 = str2;
  action = fun;
  type = t;
}

ConfigCmd::~ConfigCmd() { };


//
// check if item matches against parsed command tokens in pv (count=n)
// return from enum ( CMD_ERROR, CMD_CONF, CMD_HW, CMD_GO)
//
int ConfigCmd::Match( const char **pv, int num_pv) {
  if( num_pv < 1) return CMD_ERROR;
  if( match_string2 == NULL) {	// single match
    if( !strncasecmp( pv[0], match_string, min(strlen(pv[0]),strlen(match_string))))
      return type;
    else
      return CMD_ERROR;
  } else {
    if( num_pv < 2) return CMD_ERROR;
    if( !strncasecmp( pv[0], match_string, min(strlen(pv[0]), strlen(match_string))) &&
	!strncasecmp( pv[1], match_string2, min(strlen(pv[1]),strlen(match_string2))))
      return type;
    else
      return CMD_ERROR;
  }      
}

static const char* types[] = {"Conf", "HW", "Go" };

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




ConfigCmdType_t ConfigCmd::Type() {
  return type;
}
