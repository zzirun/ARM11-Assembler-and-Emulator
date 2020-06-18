#include "merchant.h"

#define FINALIZE_ORDER(input) \
(!strcmp(input, "end") || !strcmp(input, "END"))

#define EDIT_ORDER(input) \
(!strcmp(first_input, "e") || !strcmp(first_input, "E"))

#define INVALID_ITEM_ID(item_id, list) \
(item_id > list->tail->id || item_id < list->head->next->id)

/**
 * Adds specified quantity of items referenced by id from menu to list
 * in ascending order of id,
 * or increases quantity of order if item already in list
 * Returns true on success, false on failure
 */
bool add_order(int id, int quantity, menu_t *menu, order_list_t *order_list) {
  // Find position to add item in list (ascending order of id)
  order_t *prev = order_list->head;
	order_t *curr = order_list->head->next;
	while (curr && (curr->id < id)) {
		prev = curr;
		curr = curr->next;
	}
	if (!curr || curr->id > id) {
    // New order
    // Clone item to order from menu and assign quantity
    menu_item_t *curr_item = menu->head->next;
    while (curr_item && curr_item->id != id) {
      curr_item = curr_item->next;
    }
    order_t *order = ORDER_NEW();
    char *item_name = calloc(strlen(curr_item->name) + 1, sizeof(char));
    if (!order || !item_name) {
      perror("Failed to add menu item");
      return false;
    }
    strcpy(item_name, curr_item->name);
    order->name = item_name;
    order->id = id;
    order->price = curr_item->price;
    order->quantity = quantity;
    // Add to list in position
    order->next = curr;
    prev->next = order;
    if (!curr) {
      order_list->tail = order;
    }
    curr = order;
	} else {
    // Order already in list
    if (quantity == 0) {
      // If quantity 0, remove
      prev->next = curr->next;
      if (curr == order_list->tail) {
        order_list->tail = prev;
      }
      FREE_ORDER(curr);
    } else {
      // Else replace with new quantity of order
      curr->quantity = quantity;
    }
	}
  return true;
}


/**
 * Takes new unpaid order
 * Checks if order is empty
 * Gives options to pay now or later
 * Makes sure all unpaid orders are updated and added to unpaid list*/
void take_order(merchant_t *merchant, unpaid_t *unpaid_in) {
  unpaid_t *unpaid = unpaid_in ? unpaid_in : create_unpaid();
  order_list_t *order_list = unpaid->order_list;

	while (1) {
	  PRINT_MENU(merchant->menu, merchant->output);
    char first_input[4] = {0};
	  fprintf(merchant->output, "\nPlease input item ID or type \"end\" to finalize > ");
		fscanf(merchant->input, "%s", first_input);
		if (FINALIZE_ORDER(first_input)) {
			if (!order_list->head->next) {
        // Checks for empty order
				fprintf(merchant->output, "\n Empty order will not be saved! \n");
				break;
			} else {
        // Prints order to confirm
				printf("\n\t Your order :");
				PRINT_ORDER_LIST(order_list, merchant->output);
        // Decide to pay immediately or later
        char pay_now;
				do {
					fprintf(merchant->output, "\nPay Now? [0]No [1]Yes > ");
					fscanf(merchant->input, " %c", &pay_now);
				} while (pay_now != '0' && pay_now != '1');
        if (pay_now == '1') {
          // pay now
          pay(merchant, unpaid);
        } else {
          if (!unpaid->customer_name) {
            // add new order to list of unpaid
            add_unpaid(merchant, unpaid);
          }
          // edited orders already in unpaid list with an associated customer
        }
        break;
      }
		} else {
      // Identify item to order
      int item_id = atoi(first_input);
      if (INVALID_ITEM_ID(item_id, merchant->menu)) {
        fprintf(merchant->output, "Invalid item!\n");
        continue;
      }
      // Assign quantity
			int quantity;
			do {
				fprintf(merchant->output, "Please input quantity > ");
				fscanf(merchant->input, " %d", &quantity);
			} while (quantity < 0);
      // Add item to order
      add_order(item_id, quantity, merchant->menu, order_list);
    }
  }
}

/* Edits an unpaid order */
void edit_order(merchant_t *merchant) {
  // Get order to edit
  unpaid_t *unpaid = get_unpaid_order(merchant);
  // Check order exists and not empty
	if (!unpaid) {
		return;
	}
  assert(unpaid->order_list->head != unpaid->order_list->tail);
  // Print current order
  fprintf(merchant->output, "\nYour current order is :\n");
  PRINT_ORDER_LIST(unpaid->order_list, merchant->output);
  // Edit order
  take_order(merchant, unpaid);
}

/* Cancels unpaid order */
void cancel_order(merchant_t* merchant) {
    // Get order to cancel
	unpaid_t* unpaid = get_unpaid_order(merchant);
	// Checks if order exists and removes order
	if (unpaid) {
		remove_unpaid_order(merchant, unpaid);
		printf("Order has been removed \n");
	}
}
