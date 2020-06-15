#include "types.h"
#include "utils.c"

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

/*  Returns true on success, false on failure*/

#define MAX_MENU_ITEM_LENGTH (80)

bool parse_menu(char* path_to_menu, menu_t* menu) {
	FILE* menu_f = fopen(path_to_menu, "r");
	if (!menu_f) {
		return false;
	}
	char item[MAX_MENU_ITEM_LENGTH];
	while(fgets(item, MAX_MENU_ITEM_LENGTH, menu_f)) {
    if (!add_menu_item(item, menu)) {
      FREE_MENU(menu);
      return false;
    }
	}
	fclose(menu_f);
	return true;
}

#define INVALID_ID(item_id, orders) (item_id > orders->tail->id || item_id < orders->head->next->id)

void edit_order(order_list_t *order_list) {
	printf("\n\t Your current order is :");
	if (order_list->head == order_list->tail) {
		printf("\n\t Empty! \n");
		return;
	}
	PRINT_ORDER_LIST(order_list);
	printf("Which item do you want to change? > ");
	int id;
	scanf("%d", &id);
	while (INVALID_ID(id, order_list)) {
		printf("Invalid item ID! > ");
		scanf("%d", &id);
		printf("\n");
	}
	order_t *prev = order_list->head;
	order_t *curr = order_list->head->next;
	while (curr->id < id) {
		prev = curr;
		curr = curr->next;
	}
	printf("Please input new quantity. To delete order, set input to 0. > ");
	int new_quantity;
	scanf("%d", &new_quantity);
	if (new_quantity > 0) {
		curr->quantity = new_quantity;
	} else {
		if (curr == order_list->tail) {
			prev->next = 0;
			order_list->tail = prev;
		} else {
			prev->next = curr->next;
		}
		FREE_ORDER(curr);
	}
}

#define FINALIZE_ORDER(input) (!strcmp(input, "end") || !strcmp(input, "END"))
#define EDIT_ORDER(input) (!strcmp(first_input, "e") || !strcmp(first_input, "E"))

receipt_t *take_order(menu_t *menu, order_list_t *order_list) {
	char first_input[4];
	float total_amount = 0;
  printf("\n//////////////////////////////////////////////////////////////////\n");
	PRINT_MENU(menu);
	while (1) {
		printf("Please input item ID or type \"end\" to finalize. To edit order, type E > ");
		scanf("%s", first_input);
		if (FINALIZE_ORDER(first_input)) {
			if (total_amount == 0) {
				printf("\n\t Order is empty!\n");
				continue;
			}
			break;
		} else if (EDIT_ORDER(first_input)) {
			edit_order(order_list);
			continue;
		} else {
      int item_id = atoi(first_input);
      if (INVALID_ID(item_id, menu)) {
        printf("Invalid item!\n");
        continue;
      }
      char second_input[3];
      printf("Please input quantity > ");
      scanf(" %s", second_input);
      int quantity = atoi(second_input);
      order_t *order = order_item(menu, item_id);
      order->quantity = quantity;
      add_order(order, order_list);
      total_amount += order->price * quantity;
    }
  }
  receipt_t *receipt = make_receipt(order_list, total_amount);
	print_receipt(receipt);
	payment_t payment_type;
	do {
		printf("How do you want to pay? [0]Cash [1]Card [2]e-Wallet > ");
		scanf("%d", &payment_type);
	} while (INVALID_PAYMENT(payment_type));
	receipt->payment_type = payment_type;
	return receipt;
}

#define MAX_USER_ID_LENGTH (50)

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
