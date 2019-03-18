#ifndef HELPERS

#define HELPERS

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "phonebooks.h"
#include "definitions.h"

typedef void (*helpCB)();

extern void help(void);
extern void helpRemove(void);
extern void helpSearch(void);
extern void helpEdit(void);
extern int realySave(void);
extern int chooseOption(int fRange, int sRange, helpCB helper);


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
  fprintf(stdout, "\
\n\n1. Search By Name\n\
2. Search By Number\n\
3. Back \n\n\
Input: ");
}

extern void helpEdit(void) {
  fprintf(stdout, "\
\n\n What details do you want to edit?\n\
1. First Name\n\
2. Last Name\n\
3. Home Number \n\
4. Work Number\n\n\
Input: ");
}

extern int realySave(void) {
  int input = 0;
  scanf("%d", &input);
  return input;
}

extern int chooseOption(int fRange, int sRange, helpCB helper) {

  helper();

  int userChoice = 0 ;

  fscanf(stdin, "%d", &userChoice);

  while ( userChoice < fRange || userChoice > sRange ) {
    helper();
    fscanf(stdin, "%d", &userChoice);
  }

  return userChoice;
}


extern void editCred( struct PHONEBOOK_t * contact, char * fName , char * lName, unsigned long * work , unsigned long * home) {

  int wValid , opt = chooseOption(1, 4, helpEdit);

  PROGRAM_STATE = EDITED;

  switch(opt) {

  case 1:

    CHECK_LENGTH(fName,"Enter First Name: ");
    contact->firstName = fName;

    break;

  case 2:

    CHECK_LENGTH(lName,"Enter Last Name: ");
    contact->lastName = lName;

    break;

  case 3:

    fprintf(stdout, "Enter Work Number: ");

    if ( (wValid = fscanf(stdin, "%ld", work)) == 0 ) {
      fprintf(stderr, "invalid input\n");
      exit(1);
    }

    break;

  case 4:

    fprintf(stdout, "Enter Home Number: ");

    if ( (wValid = fscanf(stdin, "%ld", home)) == 0 ) {
      fprintf(stderr, "invalid input\n");
      exit(1);
    }

    break;

  default: break;
  }
  return;
}


#endif
