#include "merchant.h"
#include "utils.c"
#include "login.c"

receipt_t *take_order(menu_t *menu, order_list_t *order_list) {
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


char* store_receipt(char* base_path_name, receipt_t *receipt) {
  char* path_to_receipt = calloc(MAX_RECEIPT_PATH_LENGTH, sizeof(char));
  if (!path_to_receipt) {
    fprintf(stderr, "Cannot store receipt");
  }
	char* curr_time = current_time();
  // Enter receipt path identifier: merchant's (folder) name and current time of storing the receipt
	snprintf(path_to_receipt, MAX_RECEIPT_PATH_LENGTH, "%s%s", base_path_name, curr_time);
	free(curr_time);
  // Enter receipt data (order list, total, payment method)
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


