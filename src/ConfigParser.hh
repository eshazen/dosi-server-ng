// ConfigParser.hh

#include <vector>
#include "DosiConfig.hh"

// type of configuration item referenced by command
enum ConfigItemType {
  Constant,			// constant value
  Int1,				// integer from parameter 1
  Laser,			// laser enable string
  PGAMode,			// PGA mode
};

class ConfigItem {

public:

  ConfigItem( const char* str,                   ConfigItemType type, void *dptr, int dnum);
  ConfigItem( const char* str, const char* str2, ConfigItemType type, void *dptr, int dnum);
  ~ConfigItem();

  void Print();
  int Compare( char *parv[]);

private:

  const char *str1;	       // string to match first token
  const char *str2;	       // string to match second token or NULL
  ConfigItemType type;	       // item type
  void *data_ptr;	       // pointer to config item
  int data_item_num;	       // command token which contains data or constant value
};

class ConfigItemList {

public:

  ConfigItemList();
  ~ConfigItemList();
  void AddItem( ConfigItem* ip);
  void Print();

private:
  std::vector<ConfigItem> items;

};
