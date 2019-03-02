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
Work Number: %ld\n\
Home Number: %ld\n\n\
" , contacts->_id , contacts->firstName , contacts->lastName, contacts->work ? contacts->work->number : 0 , contacts->home ? contacts->home->number : 0 );

#define CHECK_LENGTH(buf,input)                                     \
  fprintf(stdout, input);                                               \
  fscanf(stdin, "%s", buf);                                             \
  while ( strlen(buf) > STRING_MAX ) {                                  \
    fprintf(stderr, "\n\nTotal length of name allowed is 20 but you provided %ld\n\n", strlen(buf)); \
    fprintf(stdout, input);                                             \
    fscanf(stdin, "%s", buf);                                           \
  }                                                                     \

#define STRING_MAX 20

#define MAKEREGEXP(op) ("(" op ")")

#define FREE(st) \
  free(contacts->firstName);                              \
  free(contacts->lastName);                               \
  free(contacts->home);                                   \
  free(contacts->work);                                   \
  free(contacts);                                         \
  contacts->firstName = NULL;                             \
  contacts->lastName  = NULL;                             \
  contacts->home      = NULL;                             \
  contacts->work      = NULL;                             \
  contacts            = NULL;


#define WRITE_TO_DB(contactDB,contacts)                                 \
  fprintf(contactDB, "ID=%d\nFIRSTNAME=%s\nLASTNAME=%s\nHOMENUMBER=%ld\nWORKNUMBER=%ld\n", contacts->_id, contacts->firstName, contacts->lastName, contacts->home->number, contacts->work->number); \

/* #define GET_KEY_TYPE(key)                       \ */
/*   if ( key == "ID" ) return 1;                  \ */
/*   if ( key == "FIRSTNAME" ) return 2;           \ */
/*   if ( key == "LASTNAME" ) return 3;            \ */
/*   if ( key == "HOMENUMBER" ) return 4;          \ */
/*   if ( key == "WORKNUMBER" ) return 5;          \ */



#endif
