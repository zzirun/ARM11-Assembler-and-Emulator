#include "types.h"
#include "utils.h"

#define MAX_USER_ID_LENGTH (50)

#define INVALID_ITEM_ID(item_id, list) \
(item_id > list->tail->id || item_id < list->head->next->id)

#define FINALIZE_ORDER(input) \
(!strcmp(input, "end") || !strcmp(input, "END"))

#define EDIT_ORDER(input) \
(!strcmp(first_input, "e") || !strcmp(first_input, "E"))

merchant_t *login_and_init(void);

void take_order(merchant_t *merchant, order_list_t *order_list);

void edit_order(merchant_t *merchant);

// To Do: Cancel order

void pay(merchant_t *merchant, order_list_t *order_list);

// To Do : Load New Menu (if have time)

// To Do: Free merchant



