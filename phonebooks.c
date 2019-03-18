#include <stdio.h>

#include "helpers.h"
#include "phonebooks.h"

int main(void) {

  struct PHONEBOOK_t * phoneBook = NULL;

  int exitLoop = 0;

  phoneBook = loadContacts(phoneBook);

  while ( 1 ) {
    switch(chooseOption(1,7,help)) {
    case 1:
      if ( ! phoneBook )
        phoneBook = addContact(phoneBook);
      else
        addContact(phoneBook);
      break;
    case 2:
      listContacts(phoneBook);
      removeContact(phoneBook);
      break;
    case 3:
      //searchByName(phoneBook);
      listContacts(phoneBook);
      editContacts(phoneBook);
      break;
    case 4:

      while ( 1 ) {

        switch(chooseOption(1, 3, helpSearch)) {

        case 1:
          searchByName(phoneBook);
          break;

        case 2:
          searchByNumber(phoneBook);
          break;

        case 3:
          exitLoop = 1;
          break;
        }

        if ( exitLoop == 1 ) break;
      }

      exitLoop = 0;

      break;

    case 5:
      saveContacts(phoneBook);
      break;
    case 6:
      listContacts(phoneBook);
      break;
    case 7:

      switch( PROGRAM_STATE ) {

      case JUSTLOADED: fprintf(stdout, "No changes were made ( Quiting...)\n "); break;

      case CREATED   :
        fprintf(stdout, "Newly added contact will not be saved. Do you want to save them ( 1 (Yes) | 2 (No) ): " );
        realySave() == 1 ? saveContacts(phoneBook) : fprintf(stdout, "Created changes will be lost") ;
        break;
      case REMOVED:
        fprintf(stdout, "Remove contacts will not be deleted. Do you want to delete them ( 1 (Yes) | 2 (No) ): " );
        realySave() == 1 ? saveContacts(phoneBook) : fprintf(stdout, "Created changes will be lost") ;
        break;
      case EDITED:
        fprintf(stdout, "Edited informations will not be save. Do you want to save them ( 1 (Yes) | 2 (No) ): " );
        realySave() == 1 ? saveContacts(phoneBook) : fprintf(stdout, "Created changes will be lost") ;
        break;
      default:
        break;
      }
      exitLoop = 1;
      break;
    }

    if ( exitLoop == 1 )
      break;
  }

  exit(0);
}
