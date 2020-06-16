#include "merchant.h"

// TO MODIFY - to fit with text file input
merchant_t *login_and_init(void) {
  /* Attempt to login */
  char* folder_path = login();
  /* Merchant's email login authentication */
  char *email = calloc(MAX_EMAIL_LENGTH, sizeof(char));
  char *password = calloc(MAX_PASSWORD_LENGTH, sizeof(char));
	fprintf(stdout, "To allow us to send email receipts to customers,\n");
	fprintf(stdout, "Please enter your email address > "); 
  //Currently only support gmail accounts that allow access to less secure apps
	// CHECK FOR WORD LIMIT?
  fscanf(stdin, "%s", email);
	fprintf(stdout, "\nPlease enter you password > ");
	fscanf(stdin, "%s", password);
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
  merchant->input = stdin;
  merchant->output = stdout;
  return merchant;
}

// DRAFT DONE, TO CHECK
void take_order(merchant_t *merchant, order_list_t *ol) {
  order_list_t* order_list = ol ? ol : ORDER_LIST_NEW(); 
	char first_input[4];
	while (1) {
    fprintf(merchant->output, "\n//////////////////////////////////////////////////////////////////\n");
	  PRINT_MENU(merchant->menu, stdout);
		fprintf(merchant->output, "Please input item ID or type \"end\" to finalize > ");
		fgets(first_input, 4, merchant->input);
    if (!first_input) {
      fprintf(merchant->output, "No input processed, please try again.\n");
      continue;
    }
		if (FINALIZE_ORDER(first_input)) {
			if (!order_list->head->next) {
				fprintf(merchant->output, "\n Empty order will not be saved! \n");
				break;
			} else {
        bool pay_now;
        do {
          fprintf(merchant->output, "\n Pay Now? [0]No [1]Yes \n");
        } while (!fscanf(merchant->input, "%d", &pay_now));    
        if (pay_now) {
          pay(merchant, order_list);
        } else {
          add_unpaid(merchant, order_list);
        }
        break;
      }
		} else {
      int item_id = atoi(first_input);
      int quantity;
      if (INVALID_ITEM_ID(item_id, merchant->menu)) {
        fprintf(merchant->output, "Invalid item!\n");
        continue;
      }
      do {
        fprintf(merchant->output, "Please input quantity > ");
      } while (!fscanf(merchant->input, "%d", &quantity));    
      add_order(item_id, quantity, merchant->menu, order_list);
    } 
  }
}

// DRAFT DONE, TO CHECK
void edit_order(merchant_t *merchant) {
  // Get order to edit
  unpaid_t *unpaid = get_unpaid_order(merchant);
  order_list_t *order_list = unpaid->order_list;
  // Check order not empty
  assert(order_list->head != order_list->tail);
  // Print current order
  fprintf(merchant->output, "\nYour current order is :\n");
  PRINT_ORDER_LIST(order_list, merchant->output);
  // Edit order
  take_order(merchant, order_list);
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
  /* Store receipt in merchant's folder */
  char* path_to_receipt = store_receipt(merchant, receipt);
  /* Send receipt to customer */
  send_receipt(merchant, path_to_receipt);
  /* Remove unpaid order from list, free memory */
  if (!ol) {
    remove_unpaid_order(merchant, unpaid);
  } else {
    FREE_ORDER_LIST(order_list);
  }
  free(path_to_receipt);
}

void free_merchant(merchant_t *merchant) {
  free(merchant->email);
  free(merchant->password);
  free(merchant->folder_path);
  FREE_MENU(merchant->menu);
  free_unpaid_list(merchant->unpaid_orders);
  free(merchant);
}