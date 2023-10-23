
#include <string.h>
#include <strings.h>
#include <cstdio>
#include "ConfigParser.hh"

ConfigItem::ConfigItem( const char*str, ConfigItemType itype, void *dptr, int dnum) {
  str1 = strdup(str);
  str2 = NULL;
  type = itype;
  data_ptr = dptr;
  data_item_num = dnum;
}

ConfigItem::ConfigItem( const char*pstr1, const char* pstr2, ConfigItemType itype, void *dptr, int dnum) {
  str1 = strdup(pstr1);
  str2 = strdup(pstr2);
  type = itype;
  data_ptr = dptr;
  data_item_num = dnum;
}


ConfigItem::~ConfigItem() {
}

void ConfigItem::Print() {
  printf("str1=\"%s\" str2=\"%s\" type=%d ptr=%p dnum=%d\n",
	 str1, str2, type, data_ptr, data_item_num);
}

int ConfigItem::Compare( char *parv[]) {
  if( strncasecmp( str1, parv[0], strlen(str1)))
    return 1;
  if( str2 && strncasecmp( str1, parv[0], strlen(str1)))
    return 1;
  return 0;
}


void ConfigItemList::AddItem( ConfigItem* ip) {
  ConfigItem i = *ip;
  items.push_back( i);
}


ConfigItemList::ConfigItemList() { }
ConfigItemList::~ConfigItemList() { }

void ConfigItemList::Print() {
  if( items.size()) {
    for( int i=0; i<items.size(); i++)
      items[i].Print();
  } else {
    printf("Empty list\n");
  }
}
