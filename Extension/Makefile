CC = gcc
CFLAGS = -g -Wall -pedantic

merchant: merchant.o Toolbox/utils.o Toolbox/login.o Toolbox/list_utils.o Toolbox/order.o Toolbox/pay_receipt.o Toolbox/unpaid_utils.o

merchant.o: merchant.c merchant.h

Toolbox/utils.o: Toolbox/utils.c Toolbox/utils.h Toolbox/types.h

Toolbox/login.o: Toolbox/login.c merchant.h

Toolbox/list_utils.o: Toolbox/list_utils.c Toolbox/types.h

Toolbox/order.o: Toolbox/order.c merchant.h

Toolbox/pay_receipt.o: Toolbox/pay_receipt.c merchant.h

Toolbox/unpaid_utils.o: Toolbox/unpaid_utils.c Toolbox/utils.h

runtest: Toolbox/runtest.o Toolbox/merchant.o Toolbox/utils.o Toolbox/login.o Toolbox/list_utils.o

Toolbox/runtest.o: Toolbox/runtest.c merchant.h

clean:
	rm -rf Toolbox/*.o merchant.o merchant runtest

.PHONY: clean
