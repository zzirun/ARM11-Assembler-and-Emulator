#include "utils.h"

/* Returns merchant data prior to login/registration, input of email, password */
merchant_t *login_and_init(char *input, char *output);
/* Takes a new order */
void take_order(merchant_t *merchant, unpaid_t *unpaid);
/* Edits a previous order */
void edit_order(merchant_t *merchant);
/* Cancels order by removing from unpaid order list */
void cancel_order(merchant_t* merchant);
/* If unpaid is NULL , order is selected from list of unpaid orders */
void pay(merchant_t *merchant, unpaid_t *unpaid);