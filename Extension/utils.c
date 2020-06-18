#include "utils.h"

char *id_data;
char *base_login_folder;
char *menu_name;
char *pay_str1;
char *pay_str2;
char *pay_str3;
char *payment_string[3];
char *receipt_base;

void init(void) {

  id_data = "merchantID.txt";

  base_login_folder = "Merchants/";

  menu_name = "menu.txt";

  pay_str1 = "Cash";
  pay_str2 = "Credit/Debit Card";
  pay_str3 = "e-Wallet";

  payment_string[0] = pay_str1;
  payment_string[1] = pay_str2;
  payment_string[2] = pay_str3;

  receipt_base = "Receipt_from_";

}

/* Returns the current time as a string, used as unique identifier for receipts */
char* current_time(void) {
	time_t rawtime;
	time(&rawtime);
  struct tm* timeinfo = localtime (&rawtime);
  char* time_str = calloc(20, sizeof(char));
	snprintf(time_str, 20, "%02d:%02d:%02d_%02d%02d%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
  return time_str;
}

/**
 * Reads a text file specified by path into a menu ADT
 * * assumes each menu item is on a different line
 * Returns true on success, false on failure
 */

bool parse_menu(char* path_to_menu, menu_t* menu) {
  if (!menu) {
    fprintf(stderr, "Cannot parse menu");
    return false;
  }
	FILE* menu_f = fopen(path_to_menu, "r");
	if (!menu_f) {
    fprintf(stderr, "Cannot open menu text file");
		return false;
	}
	char item[MAX_MENU_ITEM_LENGTH];
	while(fgets(item, MAX_MENU_ITEM_LENGTH, menu_f)) {
    if (!parse_menu_item(item, menu)) {
      fprintf(stderr, "Cannot parse menu item");
      FREE_MENU(menu);
      return false;
    }
	}
	fclose(menu_f);
	return true;
}

/**
 * Reads a line with into a menu item
 * * assumes each field in the item doesn't contain spaces
 *   and the fields are separated by spaces
 */
bool parse_menu_item(char *item_str, menu_t *menu) {
  menu_item_t *menu_item = MENU_ITEM_NEW();
  if (!menu_item) {
    return false;
  }
  // Set id
  char *field = strtok(item_str, " ");
  menu_item->id = atoi(field);
  // Set name
  field = strtok(NULL, " ");
  char *name = calloc(strlen(field) + 1, sizeof(char));
  if (!name) {
    return false;
  }
  strcpy(name, field);
  menu_item->name = name;
  // Set price
  menu_item->price = atof(strtok(NULL, " "));
  // Add to menu
	ADD_MENU_ITEM(menu_item, menu);
  return true;
}

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
 * Mode 0 : print menu; Mode 1 : print order_list
 * Prints each item's id, name, price, + quantity if order_list
 */
void print_list(list_t *list, int mode, FILE *dest) {
	list_elem_t *curr = list->head->next;
	fprintf(dest, "\n");
	while (curr) {
		if (mode) {
			fprintf(dest, "\t %-3d %-30s %8.2f %5d", curr->id, curr->name, curr->price, curr->quantity);
		} else if (!mode){
			fprintf(dest, "\t %-3d %-30s %8.2f ", curr->id, curr->name, curr->price);
		}
		fprintf(dest, "\n");
		curr = curr->next;
	}
}

// Q : Dealing with receipt making fail ??
receipt_t *make_receipt(order_list_t *order_list) {
  // Create receipt and assign orders
  receipt_t *receipt = calloc(1, sizeof(receipt_t));
  if (!receipt) {
    perror("Receipt cannot be created");
    exit(EXIT_FAILURE);
  }
	receipt->order_list = order_list;
  // Calculate price
  order_t *curr = order_list->head->next;
  while (curr) {
    receipt->total_amount += curr->price * curr->quantity;
    curr = curr->next;
  }
  return receipt;
}

void print_receipt(receipt_t *receipt, FILE* dest) {
  fprintf(dest, "\n\t Your receipt : ");
  PRINT_ORDER_LIST(receipt->order_list, dest);
  char *final_output = "Your total amount due is      :";
	fprintf(dest, "\t %-34s %8.2f\n\n", final_output, receipt->total_amount);
  if (receipt->payment_type) {
    fprintf(dest, "\nYou paid by : %s\n", payment_string[receipt->payment_type - 1]);
  }
}

char* store_receipt(merchant_t *merchant, receipt_t *receipt) {
  /* Allocate path to receipt string */
  char* path_to_receipt = calloc(MAX_RECEIPT_PATH_LENGTH, sizeof(char));
  if (!path_to_receipt) {
    fprintf(stderr, "Cannot store receipt");
    // MAKE TERMINATE FUNCTION
  }
  /* Modify receipt path name to merchant's (folder) name and current time */
	char* curr_time = current_time();
  snprintf(path_to_receipt, MAX_RECEIPT_PATH_LENGTH, "%s%s",
    merchant->folder_path, curr_time);
	free(curr_time);
  /* Enter receipt data (order list, total, payment method) */
	FILE* dest = fopen(path_to_receipt, "w");
  print_receipt(receipt, dest);
	fclose(dest);
	return path_to_receipt;
}

/* Calls Python function to authenticate email on SMTP server */
bool connect(char* email, char* password) {
	bool result = true;
	char command[256] = {0};
	snprintf(command, sizeof(command), "python3 ./send_email.py %s %s", email, password);
	printf("Trying to authenticate...\n");
	FILE* fp = popen(command, "r");
	char buffer[256] = {0};
	fgets(buffer, 256, fp);
	if (!strcmp(buffer, "Unable to authenticate\n")) {
		result = false;
	}
	pclose(fp);
	return result;
}

/* Calls Python function to send receipt as email */
void send_receipt(merchant_t *merchant, char* path_to_receipt) {
  /* Get customer email */
  char receiver[MAX_EMAIL_LENGTH] = {0};
  printf("Please enter customer email address to receive receipt > ");
  fscanf(merchant->input, "%s", receiver);
  /* Get name of issuer which is printed in the email description */
  char issuer[MAX_ID_LENGTH];
  printf("Please input name of issuer > ");
  fscanf(merchant->input, " %s", issuer);
  char subject[MAX_SUBJECT_LENGTH] = {0};
  snprintf(subject, MAX_SUBJECT_LENGTH, "%s%s", receipt_base, issuer);
  /* Send receipt */
  printf("Sending your receipt... \n");
	char args[256] = {0};
	snprintf(args, sizeof(args), "python3 ./send_email.py %s %s %s %s %s",
  merchant->email, merchant->password, receiver, subject, path_to_receipt);
	system(args);
  /* Done!! */
  printf("DONE!\n");
}

void free_receipt(receipt_t *receipt) {
  FREE_ORDER_LIST(receipt->order_list);
  free(receipt);
}

unpaid_t *get_unpaid_order(merchant_t *merchant) {
	if (merchant->unpaid_orders->no_of_unpaid == 0) {
		printf("\n No unpaid orders\n");
		return NULL;
	}
  fprintf(merchant->output, "\nUnpaid orders: \n");
  unpaid_t *curr = merchant->unpaid_orders->head->next;
  for (int i = 0; i < merchant->unpaid_orders->no_of_unpaid; i++) {
    fprintf(merchant->output, "    %d) %s\n", i, curr->customer_name);
    curr = curr->next;
  }
  int pos;
  do {
    fprintf(merchant->output, "Enter order number > ");
  } while (!fscanf(merchant->input, "%d", &pos)
    || pos > merchant->unpaid_orders->no_of_unpaid);
  curr = merchant->unpaid_orders->head->next;
  for (int i = 0; i < pos; i++, curr = curr->next);
  return curr;
}

void remove_unpaid_order(merchant_t *merchant, unpaid_t *unpaid) {
  assert(unpaid->prev);
	if (unpaid->next) {
		unpaid->prev->next = unpaid->next;
		unpaid->next->prev = unpaid->prev;
	} else {
		unpaid->prev->next = NULL;
		merchant->unpaid_orders->tail = unpaid->prev;
	}
  FREE_ORDER_LIST(unpaid->order_list);
  free(unpaid->customer_name);
  free(unpaid);
  merchant->unpaid_orders->no_of_unpaid--;
}

// TO DO DEAL WITH CALLOC FAILURE, REALLOC NAME TO SAVE SPACE
void add_unpaid(merchant_t *merchant, order_list_t *order_list, char* cust_name) {
  /* Get customer name associated with order */
	unpaid_t *unpaid = calloc(1, sizeof(unpaid_t));
	char *customer_name = calloc(MAX_CUSTOMER_NAME_LENGTH, sizeof(char));
	if (!cust_name) {
		fprintf(merchant->output, "Customer name > ");
		fscanf(merchant->input, "%s", customer_name);
		unpaid->customer_name = customer_name;
	} else {
		unpaid->customer_name = strcpy(customer_name, cust_name);
	}
  /* Initialise unpaid */
  unpaid->order_list = order_list;
  /* Add unpaid to list */
  unpaid->prev = merchant->unpaid_orders->tail;
  unpaid->prev->next = unpaid;
  merchant->unpaid_orders->tail = unpaid;
  merchant->unpaid_orders->no_of_unpaid++;
}

// DEAL WITH CALLOC FAILURE
unpaid_list_t *unpaid_list_new(void) {
  unpaid_list_t *unpaid_list = calloc(1, sizeof(unpaid_list_t));
  unpaid_t *first = calloc(1, sizeof(unpaid_t));
  unpaid_list->head = first;
  unpaid_list->tail = first;
  return unpaid_list;
}

void free_unpaid_list(unpaid_list_t *unpaid_list) {
  unpaid_t *curr = unpaid_list->head->next;
  unpaid_t *next;
  while (curr) {
    next = curr->next;
    free(curr->customer_name);
    FREE_ORDER_LIST(curr->order_list);
    free(curr);
    curr = next;
  }
  free(unpaid_list);
}
