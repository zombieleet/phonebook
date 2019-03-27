CFLAGS?=-g -std=gnu11 -Wall -Werror

compile:
	gcc ${CFLAGS} helpers.c phonebook_func.c phonebook.c -o phonebook
