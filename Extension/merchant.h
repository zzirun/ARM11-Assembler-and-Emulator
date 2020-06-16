// merchant/h

#include "types.h"
#include "utils.h"
#include "login.h"

#define MAX_USER_ID_LENGTH (50)

#define INVALID_ITEM_ID(item_id, list) \
(item_id > list->tail->id || item_id < list->head->next->id)

#define FINALIZE_ORDER(input) \
(!strcmp(input, "end") || !strcmp(input, "END"))

#define EDIT_ORDER(input) \
(!strcmp(first_input, "e") || !strcmp(first_input, "E"))

/* Returns merchant data prior to login/registration, input of email, password */
merchant_t *login_and_init(FILE *f);

/* Takes a new order */
void take_order(merchant_t *merchant, order_list_t *order_list);

/* Edits a previous order */
void edit_order(merchant_t *merchant);

// To Do: Cancel order

/* Pays an order */
/* If NULL order, order is selected from list of unpaid orders */
void pay(merchant_t *merchant, order_list_t *order_list);

// To Do : Load New Menu (if have time)

void free_merchant(merchant_t *merchant);



