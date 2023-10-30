#ifndef CONFIG_CMD_LIST_HH
#define CONFIG_CMD_LIST_HH

// ConfigCmdList.hh - manage a list of configuration commands
//
// this is basically just a vector<ConfigCmd *> with some convenience functions

#include "ConfigCmd.hh"

class ConfigCmdList {

public:

  ConfigCmdList();		// null constructor
  ~ConfigCmdList();		// null destructor
  void InitializeList();	// fill in the commands
  void AddItem( ConfigCmd *);	// add an item to the list
  void Print();			// print the list of items
  ConfigCmd* Get( int);		// retrieve an item by index
  int Size();			// get the number of items in the list
  ConfigCmd* Search( const char *pv[], int np); // search for an item

private:
  std::vector<ConfigCmd *> items;

};


#endif
