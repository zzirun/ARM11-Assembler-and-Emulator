#include "types.h"
#include "utils.h"

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

/**
 * Reads a text file specified by path into a menu ADT
 * * assumes each menu item is on a different line  
 * Returns true on success, false on failure
 */

bool parse_menu(char* path_to_menu, menu_t* menu) {
	FILE* menu_f = fopen(path_to_menu, "r");
	if (!menu_f) {
		return false;
	}
	char item[MAX_MENU_ITEM_LENGTH];
	while(fgets(item, MAX_MENU_ITEM_LENGTH, menu_f)) {
    if (!parse_menu_item(item, menu)) {
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
  // Find item in menu based on id
  menu_item_t *curr_item = menu->head->next;
	while (curr_item && curr_item->id != id) {
		curr_item = curr_item->next;
	}
  if (!curr_item) {
    fprintf(stderr, "Menu item not available");
    return false;
  }
  // Find position to add item in list (ascending order of id)
  order_t *prev;
	order_t *curr = order_list->head->next;
	while (curr && (curr->id < id)) {
		prev = curr;
		curr = curr->next;
	}
	if (!curr || curr->id > id) { 
    // New order 
    // Clone item to order from menu and assign quantity
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
	} else {
    // Increase quantity of order already in list
		curr->quantity += quantity;
	}	
  return true;
}

/**
 * To delete or change quantity of items ordered
 */
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
	while (INVALID_ITEM_ID(id, order_list)) {
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

/**
 * Mode 0 : print menu; Mode 1 : print order_list 
 * Prints each item's id, name, price, + quantity if order_list
 */
void print_list(list_t *list, int mode) {
	list_elem_t *curr = list->head->next;
	printf("\n");
	while (curr) {
		printf("\t %-3d %-30s %8.2f ", curr->id, curr->name, curr->price);
		if (mode) {
			printf("%5d", curr->quantity);
		}
		printf("\n");
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

void print_receipt(receipt_t *receipt) {
  printf("\n\t Your receipt : ");
  PRINT_ORDER_LIST(receipt->order_list);
  char *final_output = "Your total amount due is      :";
	printf("\t %-34s %8.2f\n\n", final_output, receipt->total_amount);
}

