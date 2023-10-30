#ifndef CONFIG_CMD_LIST_HH
#define CONFIG_CMD_LIST_HH

#include "ConfigCmd.hh"

class ConfigCmdList {

public:

  ConfigCmdList();
  ~ConfigCmdList();
  void AddItem( ConfigCmd *);
  void Print();
  ConfigCmd* Get( int);
  int Size();
  ConfigCmd* Search( const char *pv[], int np);

private:
  std::vector<ConfigCmd *> items;

};


#endif
