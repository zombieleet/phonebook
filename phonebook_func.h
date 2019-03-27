#ifndef PHONE_BOOK_STRUCTS
#define PHONE_BOOK_STRUCTS

enum STATE_E { JUSTLOADED = -1 , EDITED = 0 , CREATED , REMOVED , SAVED } PROGRAM_STATE;
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

struct PHONEBOOK_t * createContact(struct PHONEBOOK_t * contact, char * firstName, char * lastName ,unsigned long * work,unsigned long * home);
struct PHONEBOOK_t * addContact(struct PHONEBOOK_t * phoneBook );
void listContacts(struct PHONEBOOK_t * contacts);
void searchByName(struct PHONEBOOK_t * contacts);
void searchByNumber(struct PHONEBOOK_t * contacts);
void removeContact(struct PHONEBOOK_t * contacts);
void saveContacts( struct PHONEBOOK_t * contacts );
struct PHONEBOOK_t * loadContacts( struct PHONEBOOK_t * contacts );
void editContacts(struct PHONEBOOK_t * contacts );
void editCred(struct PHONEBOOK_t * contact, char * fName , char * lName, unsigned long * work , unsigned long * home);

#endif
