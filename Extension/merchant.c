#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

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

/* Returns true on success, false on failure*/
bool parse_menu(char* path_to_menu, list_t* menu_list) {
	FILE* menu = fopen(path_to_menu, "r");
	if (!menu) {
		return false;
	}
	char item[80];
	while(fgets(item, 80, menu)) {
		list_elem_t* elem = list_elem_new();
		elem->id = atoi(strtok(item, " "));
		char* item_name = calloc(70, sizeof(char));
		strcpy(item_name, strtok(NULL, " "));
		elem->name = item_name;
		elem->price = atof(strtok(NULL, " "));
		add_to_list(elem, menu_list);
	}
	fclose(menu);
	return true;
}

void edit_order(list_t* order_list) {
	printf("\n\t Your current order is :");
	if (order_list->head == order_list->tail) {
		printf("\n\t Empty! \n");
		return;
	}
	print_list(order_list, 1);
	printf("Which item do you want to change? > ");
	int id;
	scanf("%d", &id);
	while (id > order_list->tail->id || id < order_list->head->next->id) {
		printf("Invalid item ID! > ");
		scanf("%d", &id);
		printf("\n");
	}
	list_elem_t* prev = order_list->head;
	list_elem_t* curr = order_list->head->next;
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
		free_list_elem(curr);
	}
}

receipt_t* take_order(list_t* menu_list, list_t* order_list) {
	char first_input[3];
	float total_amount = 0;
	while (1) {
		printf("\n//////////////////////////////////////////////////////////////////\n");
		print_list(menu_list, 0);
		printf("Please input item ID or type \"end\" to finalize. To edit order, type E > ");
		scanf("%s", first_input);
		if (!strcmp(first_input, "end") || !strcmp(first_input, "END")) {
			if (total_amount == 0) {
				printf("\n\t Order is empty!\n");
				continue;
			}
			break;
		} else if (!strcmp(first_input, "e") || !strcmp(first_input, "E")){
			edit_order(order_list);
			continue;
		}
		int item_id = atoi(first_input);
		if (item_id > menu_list->tail->id) {
			printf("Invalid item!\n");
			continue;
		}
		char second_input[3];
		printf("Please input quantity > ");
		scanf(" %s", second_input);
		int quantity = atoi(second_input);
		list_elem_t* item = clone_item(menu_list, item_id);
		item->quantity = quantity;
		add_order(item, order_list);
		total_amount += item->price * quantity;
	}
	printf("\n\t Your receipt : ");
	print_list(order_list, 1);
	int payment_type;
	do {
		printf("How do you want to pay? [0]Cash [1]Card [2]e-Wallet > ");
		scanf("%d", &payment_type);
	} while (payment_type > 2);
	receipt_t* result = calloc(1, sizeof(receipt_t));
	result->order_list = order_list;
	result->total_amount = total_amount;
	result->payment_type = payment_type;
	return result;
}

void store_receipt(char* receipt_name, receipt_t* receipt) {
	char user_id[50] = {0};
	printf("Please enter user ID to store receipt > ");
	scanf("%s", user_id);
	user_id[strlen(user_id)] = '/';
	char path_name[strlen(login_folder_t[0]) + strlen(user_id) + strlen(receipt_name)];
	strcpy(path_name, login_folder_t[0]);
	strcpy(&path_name[strlen(login_folder_t[0])], user_id);
	strcpy(&path_name[strlen(login_folder_t[0]) + strlen(user_id)], receipt_name);
	FILE* fp = fopen(path_name, "w");
	while (!fp) {
		printf("Invalid user ID! Please enter a different one or register with us . > ");
		scanf("%s", user_id);
	}
	list_elem_t* curr = receipt->order_list->head->next;
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
