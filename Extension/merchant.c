#include "merchant.h"

/*
1) Login (If successful, return merchant dir path_name, else ask for registration which involves setting a password and providing a menu.txt)
2) Ask for email login authentication
3) Parse menu into linked list defined in types.h and print the menu (id, name, price)
4) Take order by specifying id and quantity (Use linked list sorted by id as well so we can change the quantity/delete item from order)
(Also keep track of total amount)
5) When the operator inputs a keyword ("END"/"end" maybe?), we print total amount, ask for payment method
(If not registered, ie not in userID.txt, then the user has to register first)
6) Ask for customer email address
(Format of receip probably some headers with shop name, date, items, total amount and payment method)
8) Print success, go back to 3)
*/


void *take_order(menu_t *menu, order_list_t *order_list) {
	char first_input[4];
	while (1) {
    printf("\n//////////////////////////////////////////////////////////////////\n");
	  PRINT_MENU(menu, stdout);
		printf("Please input item ID or type \"end\" to finalize. To edit order, type E > ");
		fgets(first_input, 4, stdin);
    if (!first_input) {
      printf("No input processed, please try again.\n");
      continue;
    }
		if (FINALIZE_ORDER(first_input)) {
			if (!order_list->head->next) {
				printf("\n\t Order is empty!\n");
				continue;
			}
			break;
		} else if (EDIT_ORDER(first_input)) {
			edit_order(order_list);
			continue;
		} else {
      int item_id = atoi(first_input);
      if (INVALID_ITEM_ID(item_id, menu)) {
        printf("Invalid item!\n");
        continue;
      }
      char second_input[3];
      printf("Please input quantity > ");
      fgets(second_input, 3, stdin);
      if (!second_input) {
        printf("No input processed, please try again.\n");
        continue;
      }
      int quantity = atoi(second_input);
      add_order(item_id, quantity, menu, order_list);
    } 
  }
  receipt_t *receipt = make_receipt(order_list);
	print_receipt(receipt, stdout);
	payment_t payment_type;
	do {
		printf("How do you want to pay? [0]Cash [1]Card [2]e-Wallet > ");
		scanf("%d", &payment_type);
	} while (INVALID_PAYMENT(payment_type));
	receipt->payment_type = payment_type;
	return receipt;
}

char* current_time(void) {
	time_t rawtime;
	time(&rawtime);
  struct tm* timeinfo = localtime (&rawtime);
  char* time_str = calloc(20, sizeof(char));
	snprintf(time_str, 20, "%02d:%02d:%02d_%02d%02d%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
  return time_str;
}


char* store_receipt(char* base_path_name, receipt_t *receipt, payment_t payment_type) {
	receipt->payment_type = payment_type;
  char* path_to_receipt = calloc(MAX_RECEIPT_PATH_LENGTH, sizeof(char));
	char* curr_time = current_time();
	snprintf(path_to_receipt, MAX_RECEIPT_PATH_LENGTH, "%s%s", base_path_name, curr_time);
	free(curr_time);
	FILE* dest = fopen(path_to_receipt, "w");
  print_receipt(receipt, dest);
	fclose(dest);
	return path_to_receipt;
}

/* Calls Python function to send receipt as email */
void send_receipt(char* sender, char* password, char* receiver, char* subject, char* path_to_receipt) {
	char arguments[256] = {0};
	snprintf(arguments, sizeof(arguments), "python3 ./send_email.py %s %s %s %s %s", sender, password, receiver, subject, path_to_receipt);
	system(arguments);
}

int main(void) {

	char termination;
	char* folder_path = login();

	printf("To allow us to send email receipts to customers,\n");
	printf("Please enter your email address > "); //Currently only support gmail accounts that allow access to less secure apps
	char email[MAX_EMAIL_LENGTH] = {0};
	scanf("%s", email);
	printf("Please enter you password > ");
	char password[MAX_PASSWORD_LENGTH] = {0};
	scanf(" %s", password);

	menu_t* menu = MENU_NEW(); 
  if (!menu) {
    perror("Cannot allocate menu");
    return EXIT_FAILURE;
  }

	do {

		order_list_t* order_list = ORDER_LIST_NEW();
		char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
		snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", folder_path, menu_name);
		parse_menu(path_to_menu, menu);
		take_order(menu, order_list);

		printf("How do you want to pay? [0]Cash [1]Card [2]e-Wallet > ");
		int payment_type;
		scanf("%d", &payment_type);

		char* path_to_receipt = store_receipt(folder_path, order_list, payment_type);

		printf("Please enter customer email address to receive receipt > ");
		char receiver[MAX_EMAIL_LENGTH] = {0};
		scanf(" %s", receiver);

		printf("Please input name of issuer > ");
		char issuer[MAX_ID_LENGTH];
		scanf(" %s", issuer);
		char subject[MAX_SUBJECT_LENGTH] = {0};
		snprintf(subject, MAX_SUBJECT_LENGTH, "%s%s", receipt_base, issuer);
		printf("Sending your receipt... \n");
		send_receipt(email, password, receiver, subject, path_to_receipt);
		printf("DONE!\n");

		FREE_ORDER_LIST(order_list);
		free(path_to_receipt);

		printf("Do you want to continue? [y/n] > ");
		scanf(" %c", &termination);
	} while (termination == 'y' || termination == 'Y');

	FREE_MENU(menu);
	free(folder_path);
	printf("Thank you for using our service! :D \n");

	return EXIT_SUCCESS;
}
