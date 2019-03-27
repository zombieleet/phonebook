#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <regex.h>

#include "phonebook_func.h"
#include "definitions.h"
#include "helpers.h"


union PHONEBOOK_l { unsigned long length ; } phonenumber_l = { 0 };

struct PHONEBOOK_t *
createContact(
              struct PHONEBOOK_t * contact ,
              char * firstName,
              char * lastName ,
              unsigned long * work,
              unsigned long * home
              )
{

  struct PHONEBOOK_t * newContact = NULL;

  if ( ! contact ) {
    contact = malloc(sizeof(struct PHONEBOOK_t));
    CHECK_MALLOC(contact);
    contact->prevContact = NULL;
    contact->nextContact = NULL;
    contact->_id = 0;
    newContact = contact;
  } else {
    contact->nextContact = malloc(sizeof(struct PHONEBOOK_t));
    CHECK_MALLOC(contact->nextContact);
    newContact = contact->nextContact;
    newContact->prevContact = contact;
    newContact->_id = contact->_id + 1;
  }

  newContact->firstName = firstName;
  newContact->lastName = lastName;

  if ( ! work )
    newContact->work = NULL;
  else {
    newContact->work = malloc(sizeof(struct NUMBER_t));
    newContact->work->number = *work;
  }

  if ( ! home )
    newContact->home = NULL;
  else {
    newContact->home = malloc(sizeof(struct NUMBER_t));
    newContact->home->number = *home;
  }

  phonenumber_l.length = phonenumber_l.length + 1;

  return newContact;
}

struct PHONEBOOK_t * addContact(struct PHONEBOOK_t * phoneBook ) {

  char * fName = malloc(STRING_MAX + sizeof(char));
  char * lName = malloc(STRING_MAX + sizeof(char));

  unsigned long workNumber = 0;
  unsigned long homeNumber = 0;

  int wValid = 0;

  CHECK_LENGTH(fName,"Enter First Name: ");
  CHECK_LENGTH(lName,"Enter Last Name: ");


  fprintf(stdout, "Enter Work Number: ");
  if ( (wValid = fscanf(stdin, "%ld", &workNumber)) == 0 ) {
    fprintf(stderr, "invalid input\n");
    exit(1);
  }

  fprintf(stdout, "Enter Home Number: ");

  if ( (wValid = fscanf(stdin, "%ld", &homeNumber)) == 0 ) {
    fprintf(stderr, "invalid input\n");
    exit(1);
  }

  unsigned long * work = workNumber == 0 ? NULL : &workNumber;
  unsigned long * home = homeNumber == 0 ? NULL : &homeNumber;

  PROGRAM_STATE = CREATED;

  if ( ! phoneBook ) {
    return createContact(NULL, fName , lName, work , home);
  }

  if ( ! phoneBook->nextContact ) {
    createContact(phoneBook, fName, lName, work, home);
    return NULL;
  }

  while ( phoneBook->nextContact ) {
    phoneBook = phoneBook->nextContact;
  }

  createContact(phoneBook, fName, lName, work, home);
  return NULL;
}

void listContacts(struct PHONEBOOK_t * contacts) {

  if ( ! contacts ) {
    fprintf(stderr, "\n\nphonebook is empty\n\n");
    return;
  }

  if ( ! contacts->nextContact ) {
    PRINT_PHONE_BOOK(contacts);
    return;
  }

  while ( contacts->nextContact ) {
    PRINT_PHONE_BOOK(contacts);
    contacts = contacts->nextContact;
  }
  PRINT_PHONE_BOOK(contacts);
  return;
};

void searchByName(struct PHONEBOOK_t * contacts) {

  if ( ! contacts ) {
    fprintf(stdout, "\n\nNo contacts have been added yet\n\n");
    return;
  }

  char * searchName = malloc(STRING_MAX + sizeof(char));
  CHECK_LENGTH(searchName,"Enter Name to search for: ");

  regex_t * phoneBookRegexp  = malloc(sizeof(regex_t));
  char    * openCaptureGroup = calloc(strlen(searchName) + sizeof(char) , sizeof(char));

  openCaptureGroup[0] = '(';

  char * regexpString    = strcat(openCaptureGroup, strcat( searchName , ")" ) );
  int    compiledRegexp  = regcomp(phoneBookRegexp, regexpString , REG_EXTENDED | REG_ICASE);

  if ( compiledRegexp != 0 ) {
    fprintf(stderr, "failed to compile regular expression");
    exit(1);
  }


  if ( ! contacts->nextContact ) {
    if (
        regexec(phoneBookRegexp, contacts->firstName, 0, NULL, 0) != REG_NOMATCH ||
        regexec(phoneBookRegexp, contacts->lastName, 0 , NULL, 0) != REG_NOMATCH
        ) {

      PRINT_PHONE_BOOK(contacts);

    } else {
      fprintf(stdout, "%s was not found in phonebook\n", searchName);
    }

    free(openCaptureGroup);
    free(searchName);
    regfree(phoneBookRegexp);
    return;
  }

  while ( contacts->nextContact ) {
    if ( regexec(phoneBookRegexp, contacts->firstName, 0, NULL, 0) != REG_NOMATCH ||
         regexec(phoneBookRegexp, contacts->lastName, 0 , NULL, 0) != REG_NOMATCH) PRINT_PHONE_BOOK(contacts);
    contacts = contacts->nextContact;
  }

  if ( regexec(phoneBookRegexp, contacts->firstName, 0, NULL, 0) != REG_NOMATCH ||
       regexec(phoneBookRegexp, contacts->lastName, 0 , NULL, 0) != REG_NOMATCH) PRINT_PHONE_BOOK(contacts);

  free(openCaptureGroup);
  free(searchName);
  regfree(phoneBookRegexp);
  return;
}

void searchByNumber(struct PHONEBOOK_t * contacts) {

  if ( ! contacts ) {
    fprintf(stdout, "\n\nNo contacts have been added yet");
    return;
  }

  unsigned long number = 0;
  int valid;

  fprintf(stdout, "Enter Phone Number: ");

  if ( ( valid = fscanf(stdin, "%ld", &number) ) == 0 ) {
    fprintf(stderr, "Invalid Input");
    exit(1);
  }

  regex_t * numberRegexp = malloc(sizeof(regex_t));

  char numStr[STRING_MAX + sizeof(char)];

  sprintf(numStr, "%ld", number); // uh ?

  int compileRegexp = regcomp(numberRegexp, numStr, 0);

  if ( compileRegexp != 0 ) {
    fprintf(stderr, "\nfailed to compile regular expression\n");
    exit(1);
  }

  char fstr[STRING_MAX + sizeof(char)];
  char sstr[STRING_MAX + sizeof(char)];

  if ( ! contacts->nextContact ) {

    sprintf(fstr, "%ld", contacts->work->number);
    sprintf(sstr, "%ld", contacts->home->number);

    if (
        regexec(numberRegexp, fstr, 0, NULL, 0) != REG_NOMATCH ||
        regexec(numberRegexp, sstr, 0 , NULL, 0) != REG_NOMATCH
        ) {

      PRINT_PHONE_BOOK(contacts);

    } else {
      fprintf(stdout, "%ld was not found in phonebook", number);
    }
    free(numberRegexp);
    return;
  }

  while ( contacts->nextContact ) {

    sprintf(fstr, "%ld", contacts->work->number);
    sprintf(sstr, "%ld", contacts->home->number);

    if ( regexec(numberRegexp, fstr, 0, NULL, 0) != REG_NOMATCH ||
         regexec(numberRegexp, sstr, 0 , NULL, 0) != REG_NOMATCH) PRINT_PHONE_BOOK(contacts);

    contacts = contacts->nextContact;

  }

  sprintf(fstr, "%ld", contacts->work->number);
  sprintf(sstr, "%ld", contacts->home->number);

  if ( regexec(numberRegexp, fstr, 0, NULL, 0) != REG_NOMATCH ||
       regexec(numberRegexp, sstr, 0 , NULL, 0) != REG_NOMATCH) PRINT_PHONE_BOOK(contacts);

  free(numberRegexp);
  return;

}

void removeContact(struct PHONEBOOK_t * contacts) {

  if ( ! contacts ) {
    fprintf(stderr, "\n\nPhonebook is empty\n\n");
    return;
  }

  unsigned long id = 0;
  int valid = 0;

  fprintf(stdout, "Enter User Id:");

  if ( ( valid = fscanf(stdin, "%ld", &id) ) == 0 ) {
    fprintf(stderr, "Invalid number input");
    exit(1);
  }

  if ( id < 0 ) {
    fprintf(stderr, "%ld is not a valid number", id);
    return;
  }

  if ( id > phonenumber_l.length ) {
    fprintf(stdout, "id of %ld does not exists", id);
    return;
  }

  PROGRAM_STATE = REMOVED;

  if ( ! contacts->nextContact && contacts->_id == id ) {
    FREE(contacts);
    phonenumber_l.length--;
    return;
  }

  int foundInLoop = 0;

  /* <--|<---|<---| */
  /*    |-- >|--->|--> */

  while ( contacts->nextContact ) {

    if ( contacts->_id == id ) {

      /* <--|<------| */
      /*    |---|-->|--> */

      contacts->nextContact->prevContact = contacts->prevContact;

      if ( contacts->prevContact )
        /* <--|<------| */
        /*    |------>|--> */
        contacts->prevContact->nextContact = contacts->nextContact;

      FREE(contacts);
      phonenumber_l.length--;
      foundInLoop = 1;
      break;
    }
    contacts = contacts->nextContact;
  }

  if ( foundInLoop == 1 ) return;

  if ( contacts->_id == id ) {
    FREE(contacts);
    phonenumber_l.length--;
    return;
  }

}

void saveContacts( struct PHONEBOOK_t * contacts ) {

  if ( ! contacts ) {
    fprintf(stderr, "\nNo contact information have been added yet\n\n" );
    return ;
  }

  FILE * contactDB = fopen("./contact.db", "w");

  if ( ! contactDB ) {
    fprintf(stderr, "\n\nCannot open file contact.db\n\n");
    return;
  }

  fprintf(stdout, "\n\nSaving Wait...\n\n");

  PROGRAM_STATE = SAVED;

  if ( ! contacts->nextContact ) {
    WRITE_TO_DB(contactDB, contacts);
    fclose(contactDB);
    fprintf(stdout, "\nDone...\n");
    return;
  }

  while ( contacts->nextContact ) {
    WRITE_TO_DB(contactDB, contacts);
    contacts = contacts->nextContact;
  }

  if ( contacts ) {
    WRITE_TO_DB(contactDB, contacts);
  }

  fclose(contactDB);
  fprintf(stdout, "\nDone...\n");
  return;
}



struct PHONEBOOK_t * loadContacts( struct PHONEBOOK_t * contacts ) {

  FILE * contactDB = fopen("./contact.db", "r");

  if ( ! contactDB ) return NULL;

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

void editContacts(struct PHONEBOOK_t * contacts ) {

  if ( ! contacts ) {
    fprintf(stdout, "\nContact list is empty\n");
    return;
  }

  unsigned long id = 0;
  int wValid;

  fprintf(stdout, "Enter the ID of the contact to edit: ");

  if ( (wValid = fscanf(stdin, "%ld", &id)) == 0 ) {
    fprintf(stderr, "invalid input\n");
    exit(1);
  }

  if ( id < 0 ) {
    fprintf(stderr, "%ld is not a valid number", id);
    return;
  }

  if ( id > phonenumber_l.length ) {
    fprintf(stdout, "id of %ld does not exists", id);
    return;
  }

  char * fName = malloc(STRING_MAX + sizeof(char));
  char * lName = malloc(STRING_MAX + sizeof(char));

  unsigned long workNumber = 0;
  unsigned long homeNumber = 0;

  int exitLoop = 0;

  if ( ! contacts->nextContact  && contacts->_id == id ) {
    editCred(contacts, fName, lName, &workNumber, &homeNumber);
    return;
  }

  while ( contacts->nextContact ) {
    if ( contacts->_id == id ) {
      editCred(contacts, fName, lName, &workNumber, &homeNumber);
      exitLoop  = 1;
      break;
    }
    contacts = contacts->nextContact;
  }

  if ( exitLoop == 1 ) return;

  if ( contacts->_id == id ) {
    editCred(contacts, fName, lName, &workNumber, &homeNumber);
    return;
  }

}

void editCred( struct PHONEBOOK_t * contact, char * fName , char * lName, unsigned long * work , unsigned long * home) {

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

    contact->work->number = *work;

    break;

  case 4:

    fprintf(stdout, "Enter Home Number: ");

    if ( (wValid = fscanf(stdin, "%ld", home)) == 0 ) {
      fprintf(stderr, "invalid input\n");
      exit(1);
    }

    contact->home->number = *home;

    break;

  default: break;
  }
  return;
}
