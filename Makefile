CFLAGS?=-g -std=gnu11 -Wall -Werror

compile:
	gcc ${CFLAGS} phonebooks.c -o phonebook
