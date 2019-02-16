#include <stdio.h>

#include "helpers.h"
#include "structs.h"

int main(void) {

  struct PHONEBOOK_t * phoneBook = NULL;

  int exitLoop   = 0;

  while ( 1 ) {
    switch(chooseOption(1,6,help)) {
    case 1:
      if ( ! phoneBook )
        phoneBook = addContact(phoneBook);
      else
        addContact(phoneBook);
      break;
    case 2:
      break;
    case 3:
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
      break;
    case 6:
      listContacts(phoneBook);
      break;
    case 7:
      exitLoop = 1;
      break;
    }

    if ( exitLoop == 1 )
      break;
  }

  exit(0);
}
