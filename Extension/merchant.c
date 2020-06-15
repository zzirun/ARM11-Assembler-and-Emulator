#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "utils.c"
#include "login.c"

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

void take_order(list_t* menu_list, list_t* order_list) {
	char first_input[3];
	while (1) {
		printf("\n//////////////////////////////////////////////////////////////////\n");
		print_list(menu_list, 0);
		printf("Please input item ID or type \"end\" to finalize. To edit order, type E > ");
		scanf("%s", first_input);
		if (!strcmp(first_input, "end") || !strcmp(first_input, "END")) {
			if (order_list->head == order_list->tail) {
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
	}
	printf("\n\t Your receipt : ");
	print_list(order_list, 1);
}

char* current_time(void) {
	time_t rawtime;
	time(&rawtime);
  struct tm* timeinfo = localtime (&rawtime);
  char* time_str = calloc(20, sizeof(char));
	snprintf(time_str, 20, "%02d:%02d:%02d_%02d%02d%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
  return time_str;
}

char* store_receipt(char* base_path_name, list_t* order_list, int payment_type) {
	char* path_to_receipt = calloc(MAX_RECEIPT_PATH_LENGTH, sizeof(char));
	char* curr_time = current_time();
	snprintf(path_to_receipt, MAX_RECEIPT_PATH_LENGTH, "%s%s", base_path_name, curr_time);
	free(curr_time);
	FILE* receipt = fopen(path_to_receipt, "w");
	list_elem_t* curr = order_list->head->next;
	float total_amount = 0;
	while (curr) {
		fprintf(receipt, "%-3d %-30s %8.2f %5d \n", curr->id, curr->name, curr->price * curr->quantity, curr->quantity);
		total_amount += curr->price * curr->quantity;
		curr = curr->next;
	}
	char* final_output = "Your total amount due is      :";
	fprintf(receipt, "\n%-34s %8.2f\n", final_output, total_amount);
	fprintf(receipt, "\nYou paid by : %s\n", payment_string[payment_type]);
	fclose(receipt);
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

	list_t* menu_list = list_new();

	do {

		list_t* order_list = list_new();
		char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
		snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", folder_path, menu_name);
		parse_menu(path_to_menu, menu_list);
		take_order(menu_list, order_list);

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

		free_list(order_list);
		free(path_to_receipt);

		printf("Do you want to continue? [y/n] > ");
		scanf(" %c", &termination);
	} while (termination == 'y' || termination == 'Y');

	free_list(menu_list);
	free(folder_path);
	printf("Thank you for using our service! :D \n");

	return EXIT_SUCCESS;
}
