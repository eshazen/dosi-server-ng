#ifndef PARSE_STRING_HH
#define PARSE_STRING_HH
// ParseString.hh - string parsing helper functions
//
// parse_string() - parse a string s into up to maxtok tokens (as argc, argv)
//   store pointers in parv
//   attempt to convert tokens to integers using strtoul (decimal or hex with 0x prefix)
//   return token count
//     NOTE:  modifies s
//
//
int parse_string( char *s, char *parv[], uint32_t *pari, int maxtok);

//
// parse_laser_string() - parse a 6-character binary laser enable string
// set boolean array ena to reflect values (if NULL, ignore)
// return 0 on success, 1 on error
//
int parse_laser_string( const char *s, bool ena[6]);


#endif
