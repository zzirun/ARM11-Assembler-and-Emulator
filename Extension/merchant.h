#include "types.h"
#include "utils.h"

#define MAX_USER_ID_LENGTH (50)

#define INVALID_ITEM_ID(item_id, list) \
(item_id > list->tail->id || item_id < list->head->next->id)

#define FINALIZE_ORDER(input) \
(!strcmp(input, "end") || !strcmp(input, "END"))

#define EDIT_ORDER(input) \
(!strcmp(first_input, "e") || !strcmp(first_input, "E"))

receipt_t *take_order(menu_t *menu, order_list_t *order_list);

void edit_order(order_list_t *order_list);

void store_receipt(char *receipt_name, receipt_t *receipt);