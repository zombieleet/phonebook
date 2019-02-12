#ifndef DEFINITIONS

#define DEFINITIONS

#define CHECK_MALLOC(ptr) \
  if ( ! ptr ) { \
    fprintf(stderr, "out of memory"); \
    exit(1); \
  }


#define PRINT_PHONE_BOOK(contacts) \
  fprintf(stdout, "\n\n\
Id:          %d\n\
First Name:  %s\n\
Last Name:   %s\n\
Work Number: %d\n\
Home Number: %d\n\n\
" , contacts->_id , contacts->firstName , contacts->lastName, contacts->work ? contacts->work->number : 0 , contacts->home ? contacts->home->number : 0 );

#define STRING_MAX 20

#endif
