#ifndef HELPERS

#define HELPERS

#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "definitions.h"

extern void help(void) {
  fprintf(stdout, "\
1. Add Contact\n\
2. Remove Contact\n\
3. Edit Contact\n\
4. Search Contact\n\
5. Save Added Contact\n\
6. List Contacts\n\
7. Quit\n\
Input: ");
}

extern void helpRemove(void) {
  fprintf(stdout,"\
1. Remove By Id\n\
2. Remove By Number\n\
Input: ");
}


extern void helpSearch(void) {
  fprintf(stdout,"\
1. Search By Name\n\
2. Search By Number\n\
Input: ");
}

extern int chooseOption() {

  help();
  
  int userChoice = 0 ;
  
  fscanf(stdin, "%d", &userChoice);

  while ( userChoice < 1 || userChoice > 6 ) {
    help();
    fscanf(stdin, "%d", &userChoice);
  }

  return userChoice;
}

#endif
