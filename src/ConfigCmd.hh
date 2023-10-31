#ifndef CONFIG_CMD_HH
#define CONFIG_CMD_HH
// ConfigCmd.hh - one DOSI configuration command
//
// Each command is defined by:
//   1 or 2 string tokens
//   a type from the enum
//   'action' function object which is executed when the command is received
//     (these are initialized using lambda expressions in ConfigCmdList::InitializeList())
//

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
// if this is modified, you have to find where ConfigCmd::action() is called
// and update the call (and any lambda expressions)
#define ConfFunc_args (const char**pv,uint32_t *pi,DosiConfig *c,DOSI_HW *hw)

// define a convenience typedef for use in function arguments
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
