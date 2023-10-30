#ifndef CONFIG_CMD_HH
#define CONFIG_CMD_HH

#include "DOSI_HW.hh"
#include "DosiConfig.hh"
#include <cstdint>
#include <functional>

// define the argument list for the "action" function called to perform command action
// these can be used in the lambda code blocks when constructing the actions
//   pv - pointer to list of (pointers to) string tokens parsed from the command
//   pi - pointer to list of integers parsed from the command
//    c - pointer to DOSI configuration object
//   hw - pointer to DOSI hardware control object
//
#define ConfFunc_args (const char**pv,uint32_t *pi,DosiConfig *c,DOSI_HW *hw)

typedef std::function<void ConfFunc_args> ConfFunc_t;

// type of command
enum ConfigCmdType_t {
  CMD_ERROR=0,
  CMD_CONF=1,
  CMD_HW=2,
  CMD_GO=3
};

class ConfigCmd {

public:
  ConfigCmd( ConfigCmdType_t, const char *, ConfFunc_t ); // single token commands
  ConfigCmd( ConfigCmdType_t, const char *, const char *, ConfFunc_t ); // two-token commands
  ~ConfigCmd();
  ConfFunc_t action;
  int Match( const char **pv, int n);
  void Print();
  ConfigCmdType_t Type();

private:
  const char* match_string;
  const char* match_string2;
  ConfigCmdType_t type;
};




#endif
