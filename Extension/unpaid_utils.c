#include "utils.h"

unpaid_t *create_unpaid(void) {
  unpaid_t *unpaid = calloc(1, sizeof(unpaid_t));
  order_list_t *order_list = ORDER_LIST_NEW();
  unpaid->order_list = order_list;
  return unpaid;
}

unpaid_t *get_unpaid_order(merchant_t *merchant) {
	if (merchant->unpaid_orders->no_of_unpaid == 0) {
		printf("\n No unpaid orders\n");
		return NULL;
	}
  fprintf(merchant->output, "\nUnpaid orders: \n");
  unpaid_t *curr = merchant->unpaid_orders->head->next;
  for (int i = 0; i < merchant->unpaid_orders->no_of_unpaid; i++) {
    fprintf(merchant->output, "    %d) %s\n", i, curr->customer_name);
    curr = curr->next;
  }
  int pos;
  do {
    fprintf(merchant->output, "Enter order number > ");
  } while (!fscanf(merchant->input, "%d", &pos)
    || pos > merchant->unpaid_orders->no_of_unpaid);
  curr = merchant->unpaid_orders->head->next;
  for (int i = 0; i < pos; i++, curr = curr->next);
  return curr;
}

void remove_unpaid_order(merchant_t *merchant, unpaid_t *unpaid) {
  assert(unpaid->prev);
	if (unpaid->next) {
		unpaid->prev->next = unpaid->next;
		unpaid->next->prev = unpaid->prev;
	} else {
		unpaid->prev->next = NULL;
		merchant->unpaid_orders->tail = unpaid->prev;
	}
  FREE_ORDER_LIST(unpaid->order_list);
  free(unpaid->customer_name);
  free(unpaid);
  merchant->unpaid_orders->no_of_unpaid--;
}

/* Adds new unpaid orders into the list, prompts for a customer name */
void add_unpaid(merchant_t *merchant, unpaid_t *unpaid) {
  /* Get customer name associated with new order */
  char *customer_name = calloc(MAX_CUSTOMER_NAME_LENGTH, sizeof(char));
  fprintf(merchant->output, "Customer name > ");
  fscanf(merchant->input, "%s", customer_name);
  unpaid->customer_name = customer_name;
  /* Add unpaid order to list */
  unpaid->prev = merchant->unpaid_orders->tail;
  unpaid->prev->next = unpaid;
  merchant->unpaid_orders->tail = unpaid;
  merchant->unpaid_orders->no_of_unpaid++;  
}

// DEAL WITH CALLOC FAILURE
unpaid_list_t *unpaid_list_new(void) {
  unpaid_list_t *unpaid_list = calloc(1, sizeof(unpaid_list_t));
  unpaid_t *first = calloc(1, sizeof(unpaid_t));
  unpaid_list->head = first;
  unpaid_list->tail = first;
  return unpaid_list;
}

void free_unpaid_list(unpaid_list_t *unpaid_list) {
  unpaid_t *curr = unpaid_list->head;
  unpaid_t *next;
  while (curr) {
    next = curr->next;
    free(curr->customer_name);
    FREE_ORDER_LIST(curr->order_list);
    free(curr);
    curr = next;
  }
  free(unpaid_list);
}
