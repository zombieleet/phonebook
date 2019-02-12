#ifndef PHONE_BOOK_STRUCTS

#define PHONE_BOOK_STRUCTS

#include <ctype.h>
#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"

struct NUMBER_t { int number; };

struct PHONEBOOK_t {

  int _id;
  char * firstName;
  char * lastName;

  struct NUMBER_t * work;
  struct NUMBER_t * home;

  struct PHONEBOOK_t * nextContact;
  struct PHONEBOOK_t * prevContact;

};

extern struct PHONEBOOK_t * createContact(struct PHONEBOOK_t * contact , char * firstName, char * lastName , int * work, int * home) {

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

  int workNumber = 0;
  int homeNumber = 0;

  int wValid = 0;

  fprintf(stdout , "Enter First Name: ");
  fscanf(stdin, "%s", fName);

  fprintf(stdout, "Enter Last Name: ");
  fscanf(stdin, "%s", lName);

  fprintf(stdout, "Enter Work Number: ");
  if ( (wValid = fscanf(stdin, "%d", &workNumber)) == 0 ) {
    fprintf(stderr, "invalid input\n");
    exit(1);
  }

  fprintf(stdout, "Enter Home Number: ");

  if ( (wValid = fscanf(stdin, "%d", &homeNumber)) == 0 ) {
    fprintf(stderr, "invalid input\n");
    exit(1);
  }

  int * work = workNumber == 0 ? NULL : &workNumber;
  int * home = homeNumber == 0 ? NULL : &homeNumber;

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

#endif
