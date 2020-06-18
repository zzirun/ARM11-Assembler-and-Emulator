CC = gcc
CFLAGS = -g -Wall -pedantic

merchant: merchant.o utils.o login.o list_utils.o order.o pay_receipt.o unpaid_utils.o

merchant.o: merchant.c merchant.h

utils.o: utils.c utils.h types.h

login.o: login.c merchant.h

list_utils.o: list_utils.c types.h

order.o: order.c merchant.h

pay_receipt.o: pay_receipt.c merchant.h

unpaid_utils.o: unpaid_utils.c utils.h

runtest: runtest.o merchant.o utils.o login.o list_utils.o

runtest.o: runtest.c merchant.h

clean:
	rm -rf *.o merchant runtest

.PHONY: clean
