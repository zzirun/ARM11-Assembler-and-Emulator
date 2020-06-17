CC = gcc
CFLAGS = -g -Wall -pedantic

main: main.o merchant.o utils.o login.o list_utils.o

main.o: main.c merchant.h

merchant.o: merchant.c merchant.h

utils.o: utils.c utils.h types.h

login.0: login.c login.h

list_utils.o: list_utils.c types.h

runtest: runtest.o merchant.o utils.o login.o list_utils.o

runtest.o: runtest.c merchant.h

clean:
	rm -rf *.o main runtest

.PHONY: clean
