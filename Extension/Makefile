CC = gcc
CFLAGS = -g -Wall -pedantic

main: main.o merchant.o utils.o login.o list_utils.o

main.o: main.c merchant.h

merchant.o: merchant.c merchant.h

utils.o: utils.c utils.h

login.0: login.c login.h

list_utils.o: types.h

clean:
	rm -rf login

.PHONY: clean
