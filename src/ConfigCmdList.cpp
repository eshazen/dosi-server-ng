#include <cstdio>
#include "ConfigCmdList.hh"

ConfigCmdList::ConfigCmdList() { };
ConfigCmdList::~ConfigCmdList() { };

void ConfigCmdList::Print() {
  if( items.size()) {
    for( int i=0; i<items.size(); i++)
      items[i]->Print();
  } else {
    printf("Empty list\n");
  }
}

void ConfigCmdList::AddItem( ConfigCmd *it) {
  items.push_back( it);
}

int ConfigCmdList::Size() {
  return items.size();
}


ConfigCmd* ConfigCmdList::Get( int i)
{
  return items[i];
}


ConfigCmd* ConfigCmdList::Search( const char *pv[], int np) {
  int it;
  for( int i=0; i<Size(); i++)
    if( it = items[i]->Match( pv, np))
      return Get(i);
  return NULL;
}
