#include "merchant.h"
#include "utils.c"
#include "login.c"

merchant_t *login_and_init(void) {
  /* Attempt to login */
  char* folder_path = login();
  /* Merchant's email login authentication */
  char *email = calloc(MAX_EMAIL_LENGTH, sizeof(char));
  char *password = calloc(MAX_PASSWORD_LENGTH, sizeof(char));
	printf("To allow us to send email receipts to customers,\n");
	printf("Please enter your email address > "); 
  //Currently only support gmail accounts that allow access to less secure apps
	fgets(email, MAX_EMAIL_LENGTH, stdin);
	printf("Please enter you password > ");
	fgets(password, MAX_PASSWORD_LENGTH, stdin);
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
  merchant->unpaid_orders = calloc(1, sizeof(unpaid_t));
}

void take_order(merchant_t *merchant, order_list_t *ol) {
  order_list_t* order_list = ol ? ol : ORDER_LIST_NEW(); 
	char first_input[4];
	while (1) {
    printf("\n//////////////////////////////////////////////////////////////////\n");
	  PRINT_MENU(merchant->menu, stdout);
		printf("Please input item ID or type \"end\" to finalize > ");
		fgets(first_input, 4, stdin);
    if (!first_input) {
      printf("No input processed, please try again.\n");
      continue;
    }
		if (FINALIZE_ORDER(first_input)) {
			if (!order_list->head->next) {
				printf("\n Empty order will not be saved! \n");
				break;
			} else {
        bool pay_now;
        do {
          printf("\n Pay Now? [0]No [1]Yes \n");
        } while (!scanf("%d", &pay_now)); 
        // TODO: store in unpaid list of orders       
        if (pay_now) {
          pay(merchant, order_list);
        }
        break;
      }
		} else {
      int item_id = atoi(first_input);
      int quantity;
      if (INVALID_ITEM_ID(item_id, merchant->menu)) {
        printf("Invalid item!\n");
        continue;
      }
      do {
        printf("Please input quantity > ");
      } while (!scanf("%d", &quantity));    
      add_order(item_id, quantity, merchant->menu, order_list);
    } 
  }
}

void edit_order(merchant_t *merchant) {
  // TO DO : FIND WAY OF GETTING ORDER FROM ID
  order_list_t *order_list;
  // Check order not empty
  assert(order_list->head != order_list->tail);
  // Print current order
  printf("\n\t Your current order is :");
  PRINT_ORDER_LIST(order_list, stdout);
  take_order(merchant, order_list);
}

void pay(merchant_t *merchant, order_list_t *order_list) {
  // TO DO Print if ORDER LIST IS NULL, ie Pay later
  receipt_t *receipt = make_receipt(order_list);
  print_receipt(receipt, stdout);
  payment_t payment_type;
  do {
    printf("How do you want to pay? [1]Cash [2]Card [3]e-Wallet > ");
  } while (!scanf("%d", &payment_type) || INVALID_PAYMENT(payment_type));	
  receipt->payment_type = payment_type;
  /* Store receipt in merchant's folder */
  char* path_to_receipt = store_receipt(merchant->folder_path, receipt);
  /* Send receipt to customer */
  send_receipt(merchant, path_to_receipt);
  /* Remove unpaid order from list, free memory */
  FREE_ORDER_LIST(order_list);
  free(path_to_receipt);
}