#ifndef PHONE_BOOK_STRUCTS

#define PHONE_BOOK_STRUCTS

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <regex.h>

#include "definitions.h"
#include "helpers.h"

struct NUMBER_t { unsigned long number; };

struct PHONEBOOK_t {

  int _id;
  char * firstName;
  char * lastName;

  struct NUMBER_t * work;
  struct NUMBER_t * home;

  struct PHONEBOOK_t * nextContact;
  struct PHONEBOOK_t * prevContact;

};

extern struct PHONEBOOK_t *
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
    newContact = malloc(sizeof(struct PHONEBOOK_t));
    CHECK_MALLOC(newContact);
    newContact->prevContact = NULL;
    newContact->nextContact = NULL;
    newContact->_id = 0;
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
  return newContact;
}

extern struct PHONEBOOK_t * addContact(struct PHONEBOOK_t * phoneBook ) {

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

extern void listContacts(struct PHONEBOOK_t * contacts) {

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
      fprintf(stdout, "%s was not found in phonebook", searchName);
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

void searchByNumber() {
}

#endif
