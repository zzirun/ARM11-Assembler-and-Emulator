#include "utils.h"

char *id_data;
char *base_login_folder;
char *menu_name;
char *payment_string[3];
char *receipt_base;

void init(void) {
  id_data = "merchantID.txt";

  base_login_folder = "Merchants/";

  menu_name = "menu.txt";

  payment_string = {"Cash","Credit/Debit Card", "e-Wallet"};

  receipt_base = "Receipt_from_";

}

/*Lists all files in the directory specified by path, printed with indent */
void list_all_files(DIR *path) {
	struct dirent *dir;
	if (!path) {
		perror("Invalid path");
		exit(EXIT_FAILURE);
	}
	while ((dir = readdir(path))) {
		if (strcmp(dir->d_name, "..") && strcmp(dir->d_name, ".")) {
			printf("  %s\n", dir->d_name);
		}
	}
}

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
  order_t *prev;
	order_t *curr = order_list->head->next;
	while (curr && (curr->id < id)) {
		prev = curr;
		curr = curr->next;
	}
	if (!curr || curr->id > id) { 
    if (quantity == 0) {
      fprintf(stderr, "Empty item");
      return true;
    }
    // New order 
    // Clone item to order from menu and assign quantity
    menu_item_t *curr_item = menu->head->next;
    while (curr_item && curr_item->id != id) {
      curr_item = curr_item->next;
    }
    if (!curr_item) {
      fprintf(stderr, "Menu item not available");
      return false;
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
    curr = order;
	} else {
    // Order already in list
    if (curr->quantity == 0) {
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
		fprintf(dest, "\t %-3d %-30s %8.2f ", curr->id, curr->name, curr->price);
		if (mode) {
			fprintf(dest, "%5d", curr->quantity);
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
  while (!curr) {
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
    fprintf(dest, "\nYou paid by : %s\n", payment_string[receipt->payment_type]);
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

/* Calls Python function to send receipt as email */
void send_receipt(merchant_t *merchant, char* path_to_receipt) {
  /* Get customer email */
  char receiver[MAX_EMAIL_LENGTH] = {0};
  printf("Please enter customer email address to receive receipt > ");
  scanf("%s", receiver);
  /* Get name of issuer which is printed in the email description */
  char issuer[MAX_ID_LENGTH];
  printf("Please input name of issuer > ");
  scanf(" %s", issuer);
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

unpaid_t *get_unpaid_order(merchant_t *merchant) {
  fprintf(merchant->output, "Unpaid orders: \n");
  unpaid_t *curr = merchant->unpaid_orders->head->next;
  for (int i = 0; i < merchant->unpaid_orders->no_of_unpaid; i++) {
    fprintf(merchant->output, "%d) %s\n", i, curr->customer_name);
    curr = curr->next;
  }
  int pos;
  do {
    fprintf(merchant->output, "Enter order number to be paid > ");
  } while (!fscanf(merchant->input, "%d", &pos) 
    || pos > merchant->unpaid_orders->no_of_unpaid);
  curr = merchant->unpaid_orders->head->next;
  for (int i = 0; i < pos; i++, curr = curr->next);
  return curr;
}

void remove_unpaid_order(merchant_t *merchant, unpaid_t *unpaid) {
  assert(unpaid->prev);
  unpaid->prev->next = unpaid->next;
  if (unpaid->next) {
    unpaid->next->prev = unpaid->prev;
  }
  FREE_ORDER_LIST(unpaid->order_list);
  free(unpaid->customer_name);
  merchant->unpaid_orders->no_of_unpaid--;
}

// TO DO DEAL WITH CALLOC FAILURE, REALLOC NAME TO SAVE SPACE
void add_unpaid(merchant_t *merchant, order_list_t *order_list) {
  /* Get customer name associated with order */
  char *customer_name = calloc(MAX_CUSTOMER_NAME_LENGTH, sizeof(char));
  do {
    fprintf(merchant->output, "Customer name > ");
  } while (!fscanf(merchant->input, "%s", customer_name));
  /* Initialise unpaid */  
  unpaid_t *unpaid = calloc(1, sizeof(unpaid_t));
  unpaid->customer_name = customer_name;
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
  unpaid_t *curr = unpaid_list->head;
  unpaid_t *next;
  while (!curr) {
    next = curr->next;
    free(curr->customer_name);
    FREE_ORDER_LIST(curr->order_list);
    free(curr);
    curr = next;
  }
  free(unpaid_list);
}