//
// parse a string s into up to maxtok tokens (as argc, argv)
// store pointers in parv
// attempt to convert tokens to integers using strtoul (decimal or hex with 0x prefix)
// return token count
//
// NOTE:  modifies s
//

int parse_string( char *s, char *parv[], uint32_t *pari, int maxtok) {
  char *p;
  if( s == NULL || iscntrl(*s))  return 0; /* ignore null strings */
  // strip trailing control characters
  while( strlen(s) && iscntrl(s[strlen(s)-1]))
    s[strlen(s)-1] = '\0';
  int i = 0;
  parv[i] = strtok( s, " ");	/* parse the first token */
  pari[i] = strtoul( parv[i], NULL, 0);
  for( i=1; i<maxtok; i++) {	/* parse remaining tokens */
    if( (p = strtok( NULL, " "))) {
      parv[i] = p;
      pari[i] = strtoul( parv[i], NULL, 0);
    } else
      break;
  }
  return i;
}
