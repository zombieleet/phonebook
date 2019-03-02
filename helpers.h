#ifndef HELPERS

#define HELPERS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "definitions.h"

typedef void (*helpCB)();

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


extern struct PHONEBOOK_t * loadContacts( struct PHONEBOOK_t * contacts ) {
  FILE * contactDB = fopen("./contact.db", "r");
  if ( ! contactDB ) {
    fprintf(stderr, "\n\nCannot open file contact.db\n\n");
    return NULL;
  }

  if ( feof(contactDB) ) {
    fprintf(stdout,"\n\n Contact list is empty. Follow the below options to create a list\n\n");
    return NULL;
  }

  size_t bytesToRead = 0;
  size_t readBytes = 0;
  char * readLine = NULL;

  char * firstName = NULL;
  char * lastName  = NULL;

  unsigned long work = 0;
  unsigned long home = 0;

  while ( ( readBytes = getline(&readLine, &bytesToRead, contactDB) ) != -1 ) {

    char * paramKeys = malloc(strlen(readLine));
    char * paramValues = malloc(strlen(readLine));

    int findEqual = 0;

    size_t i = 0 , j = 0;

    // keys
    // and
    // values
    for ( ; i < strlen(readLine) ; i++ ) {

      if ( findEqual == 1 ) {
        if ( readLine[i] == '\n') paramValues[j++] = '\0';
        paramValues[j++] = readLine[i];
        continue;
      }

      if ( readLine[i] == '=' ) {
        findEqual = 1;
        paramKeys[i] = '\0';
        continue;
      }
      paramKeys[i] = readLine[i];
    }

    paramKeys   = realloc(paramKeys, strlen(paramKeys) + 1);
    paramValues = realloc(paramValues, strlen(paramValues) + 1);

    if (strcmp(paramKeys,"ID") == 0)         free(paramKeys);
    if (strcmp(paramKeys,"FIRSTNAME") == 0)  firstName = paramValues;
    if (strcmp(paramKeys,"LASTNAME") == 0)   lastName = paramValues;
    if (strcmp(paramKeys,"HOMENUMBER") == 0) home = strtoul(paramValues, NULL , 10);

    if (strcmp(paramKeys,"WORKNUMBER") == 0) {

      work = strtoul(paramValues, NULL , 10);

      if ( ! contacts ) {
        contacts = createContact(contacts, firstName, lastName, &work, &home);
      } else
        createContact(contacts, firstName, lastName, &work, &home);
    }
  }

  free(readLine);
  fclose(contactDB);
  return contacts;
}

#endif
