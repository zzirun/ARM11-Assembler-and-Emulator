#include "merchant.h"
//#include <stdio.h>

// TO MODIFY - to fit with text file input
// For interactive mode ensure f = stdin
// For testing mode f = test file
merchant_t *login_and_init(FILE *f) {
  /* Attempt to login */
  char* folder_path = login(f);
  /* Merchant's email login authentication */
	//Currently only support gmail accounts that allow access to less secure apps
  char *email = calloc(MAX_EMAIL_LENGTH, sizeof(char));
  char *password = calloc(MAX_PASSWORD_LENGTH, sizeof(char));
  if (f == stdin) {
      fprintf(stdout, "To allow us to send email receipts to customers,\n");
      fprintf(stdout, "Please enter your email address > ");
			fscanf(f, "%s", email);
			fprintf(stdout, "Please enter your password > ");
			fscanf(f, "%s", password);
			while (!connect(email, password)) {
				printf("Authentication error!\n\n");
				fprintf(stdout, "Please enter your email address > ");
				fscanf(f, "%s", email);
				fprintf(stdout, "Please enter your password > ");
				fscanf(f, "%s", password);
				printf("Authentication successful!\n");
			}
  } else {
		fscanf(f, "%s", email);
		fscanf(f, "%s", password);
		printf("Authentication successful!\n");
	}
  /* Make merchant's menu */
	menu_t* menu = MENU_NEW();
  char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
	snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", folder_path, menu_name);
  parse_menu(path_to_menu, menu);
  /* Initialise merchant */
  merchant_t *merchant = calloc(1, sizeof(merchant_t));
  merchant->email = email;
  merchant->password = password;
  merchant->folder_path = folder_path;
  merchant->menu = menu;
  merchant->unpaid_orders = unpaid_list_new();
  merchant->input = f;
  merchant->output = stdout;
  return merchant;
}

// DRAFT DONE, TO CHECK
void take_order(merchant_t *merchant, order_list_t *ol, char* customer_name) {
  order_list_t* order_list = ol ? ol : ORDER_LIST_NEW();
	char first_input[4] = {0};
	while (1) {
    fprintf(merchant->output, "\n******************************************************************\n");
	  PRINT_MENU(merchant->menu, stdout);
		fprintf(merchant->output, "\n******************************************************************\n");
	  fprintf(merchant->output, "\nPlease input item ID or type \"end\" to finalize > ");
		fscanf(merchant->input, "%s", first_input);
		if (FINALIZE_ORDER(first_input)) {
			if (!order_list->head->next) {
				fprintf(merchant->output, "\n Empty order will not be saved! \n");
				break;
			} else {
				printf("\n\t Your receipt :");
				PRINT_ORDER_LIST(order_list, merchant->output);
        char pay_now;
				do {
					fprintf(merchant->output, "\nPay Now? [0]No [1]Yes > ");
					fscanf(merchant->input, " %c", &pay_now);
				} while (pay_now != '0' && pay_now != '1');
        if (pay_now == '1') {
          pay(merchant, order_list);
        } else {
          add_unpaid(merchant, order_list, customer_name);
        }
        break;
      }
		} else {
      int item_id = atoi(first_input);
      if (INVALID_ITEM_ID(item_id, merchant->menu)) {
        fprintf(merchant->output, "Invalid item!\n");
        continue;
      }
			int quantity;
			do {
				fprintf(merchant->output, "Please input quantity > ");
				fscanf(merchant->input, " %d", &quantity);
			} while (quantity < 0);
      add_order(item_id, quantity, merchant->menu, order_list);
    }
  }
}

// DRAFT DONE, TO CHECK
void edit_order(merchant_t *merchant) {
  // Get order to edit
  unpaid_t *unpaid = get_unpaid_order(merchant);
	if (!unpaid) {
		return;
	}
  order_list_t *order_list = unpaid->order_list;
  // Check order not empty
  assert(order_list->head != order_list->tail);
  // Print current order
  fprintf(merchant->output, "\nYour current order is :\n");
  PRINT_ORDER_LIST(order_list, merchant->output);
  // Edit order
  take_order(merchant, order_list, unpaid->customer_name);
  // Remove previous order
  remove_unpaid_order(merchant, unpaid);
}

// DRAFT DONE, TO CHECK
void pay(merchant_t *merchant, order_list_t *ol) {
  order_list_t *order_list;
  unpaid_t *unpaid = NULL;
  if (ol) {
    // pay now
    order_list = ol;
  } else {
    // NULL for pay later
    unpaid = get_unpaid_order(merchant);
		if (!unpaid) {
			return;
		}
    order_list = unpaid->order_list;
  }
  receipt_t *receipt = make_receipt(order_list);
  print_receipt(receipt, merchant->output);
  int payment_type;
  do {
    fprintf(merchant->output, "How do you want to pay? [1]Cash [2]Card [3]e-Wallet > ");
  } while (!fscanf(merchant->input,"%d", &payment_type)
            || INVALID_PAYMENT(payment_type));
  receipt->payment_type = payment_type;
  //Store receipt in merchant's folder
  char* path_to_receipt = store_receipt(merchant, receipt);
  //Send receipt to customer
  send_receipt(merchant, path_to_receipt);
  //Remove unpaid order from list, free memory
  if (!ol) {
    remove_unpaid_order(merchant, unpaid);
  }
  free(path_to_receipt);
}

/*
void pay(merchant_t *merchant, order_list_t *ol) {
  unpaid_t *unpaid = get_unpaid_order(merchant);
	if (!unpaid) {
		return;
	}
	order_list_t *order_list = unpaid->order_list;
  receipt_t *receipt = make_receipt(order_list);
  print_receipt(receipt, merchant->output);
  int payment_type;
  do {
    fprintf(merchant->output, "How do you want to pay? [1]Cash [2]Card [3]e-Wallet > ");
  } while (!fscanf(merchant->input,"%d", &payment_type)
            || INVALID_PAYMENT(payment_type));
  receipt->payment_type = payment_type;
  // Store receipt in merchant's folder
  char* path_to_receipt = store_receipt(merchant, receipt);
  // Send receipt to customer
  send_receipt(merchant, path_to_receipt);
  // Remove unpaid order from list, free memory
  remove_unpaid_order(merchant, unpaid);
	//Order_list freed by remove_unpaid_order
	free(receipt);
  free(path_to_receipt);
}
*/

void cancel_order(merchant_t* merchant) {
	unpaid_t* unpaid = get_unpaid_order(merchant);
	if (unpaid) {
		remove_unpaid_order(merchant, unpaid);
		printf("Order has been removed \n");
	}
}

void free_merchant(merchant_t *merchant) {
  free(merchant->email);
  free(merchant->password);
  free(merchant->folder_path);
  FREE_MENU(merchant->menu);
  free_unpaid_list(merchant->unpaid_orders);
	free(merchant);
}
