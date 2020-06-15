#include "merchant.h"

/*

1) Login (If successful, return merchant dir, else ask for registration which involves setting a password and providing a menu.txt)
2) Parse menu into linked list defined in types.h and print the menu (id, name, price)
3) Take order by specifying id and quantity (Use linked list sorted by id as well so we can change the quantity/delete item from order)
(Also keep track of total amount)
4) When the operator inputs a keyword ("END"/"end" maybe?), we print total amount, ask for payment method and user id
(If not registered, ie not in userID.txt, then the user has to register first)
6) Write receipt into a text file with name (<shop> <date>)
(Format of receip probably some headers with shop name, date, items, total amount and payment method)
7) Print success, go back to 3)


*/

receipt_t *take_order(menu_t *menu, order_list_t *order_list) {
	char first_input[4];
  printf("\n//////////////////////////////////////////////////////////////////\n");
	PRINT_MENU(menu);
	while (1) {
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
	print_receipt(receipt);
	payment_t payment_type;
	do {
		printf("How do you want to pay? [0]Cash [1]Card [2]e-Wallet > ");
		scanf("%d", &payment_type);
	} while (INVALID_PAYMENT(payment_type));
	receipt->payment_type = payment_type;
	return receipt;
}

void store_receipt(char *receipt_name, receipt_t *receipt) {
	char user_id[MAX_USER_ID_LENGTH] = {0};
	printf("Please enter user ID to store receipt > ");
	scanf("%s", user_id);
	user_id[strlen(user_id)] = '/';
	char path_name[strlen(login_folder_t[0]) + strlen(user_id) + strlen(receipt_name)];
	strcpy(path_name, login_folder_t[0]);
	strcpy(&path_name[strlen(login_folder_t[0])], user_id);
	strcpy(&path_name[strlen(login_folder_t[0]) + strlen(user_id)], receipt_name);
	FILE *fp = fopen(path_name, "w");
	while (!fp) {
		printf("Invalid user ID! Please enter a different one or register with us . > ");
		scanf("%s", user_id);
	}
	list_elem_t *curr = receipt->order_list->head->next;
	while (curr) {
		fprintf(fp, "%-3d %-30s %8.2f %5d \n", curr->id, curr->name, curr->price * curr->quantity, curr->quantity);
		curr = curr->next;
	}
	char* final_output = "Your total amount due is      :";
	fprintf(fp, "\n%-34s %8.2f\n", final_output, receipt->total_amount);
	fprintf(fp, "\nYou paid by : %s\n", payment_string[receipt->payment_type]);
}

/*
time_t current = time(NULL);
struct tm* pLocal = localtime(&current);
strftime(buffer, sizeof(buffer), "%d %B %Y", pLocal);
*/

int main(void) {
	list_t* menu_list = list_new();
	list_t* order_list = list_new();
	parse_menu("Merchants/bhsz/menu.txt", menu_list);
	receipt_t* result = take_order(menu_list, order_list);
	store_receipt("bhsz_140620" ,result);
	free_list(menu_list);
	free_list(order_list);
	free(result);
	return EXIT_SUCCESS;
}
