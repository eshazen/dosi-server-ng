//
// parse a string s into up to maxtok tokens (as argc, argv)
// store pointers in parv
// attempt to convert tokens to integers using strtoul (decimal or hex with 0x prefix)
// return token count
//
// NOTE:  modifies s
//

int parse_string( char *s, char *parv[], uint32_t *pari, int maxtok);
