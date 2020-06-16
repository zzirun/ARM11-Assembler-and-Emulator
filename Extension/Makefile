CC = gcc
CFLAGS = -g -Wall -pedantic

merchant: merchant.o utils.o

merchant.o: merchant.c

utils.o: utils.c types.h

login: login.c

clean:
	rm -rf login

.PHONY: clean
