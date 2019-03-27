#ifndef HELPERS
#define HELPERS

typedef void (*helpCB)();

void help(void);
void helpRemove(void);
void helpSearch(void);
void helpEdit(void);
int realySave(void);
int chooseOption(int fRange, int sRange, helpCB helper);

#endif
