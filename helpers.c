#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "definitions.h"

void help(void) {
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

void helpRemove(void) {
  fprintf(stdout,"\
1. Remove By Id\n\
2. Remove By Number\n\
Input: ");
}


void helpSearch(void) {
  fprintf(stdout, "\
\n\n1. Search By Name\n\
2. Search By Number\n\
3. Back \n\n\
Input: ");
}

void helpEdit(void) {
  fprintf(stdout, "\
\n\n What details do you want to edit?\n\
1. First Name\n\
2. Last Name\n\
3. Home Number \n\
4. Work Number\n\n\
Input: ");
}

int realySave(void) {
  int input = 0;
  scanf("%d", &input);
  return input;
}

int chooseOption(int fRange, int sRange, helpCB helper) {

  helper();

  int userChoice = 0 ;

  fscanf(stdin, "%d", &userChoice);

  while ( userChoice < fRange || userChoice > sRange ) {
    helper();
    fscanf(stdin, "%d", &userChoice);
  }

  return userChoice;
}
