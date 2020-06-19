#include "utils.h"

// Max length of a line describing a menu item from the text file
#define MAX_MENU_ITEM_LENGTH (80)
//Line divider for display purposes when a menu/order is printed
#define LINE_DIVIDER \
("\n******************************************************************\n")

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
 * Mode 0 : print menu; Mode 1 : print order_list
 * Prints each item's id, name, price, + quantity if order_list
 */
void print_list(list_t *list, int mode, FILE *dest) {
  fprintf(dest, LINE_DIVIDER);
	list_elem_t *curr = list->head->next;
	while (curr) {
		if (mode) {
			fprintf(dest, "\t %-3d %-30s %8.2f %5d", curr->id, curr->name, curr->price, curr->quantity);
		} else if (!mode){
			fprintf(dest, "\t %-3d %-30s %8.2f ", curr->id, curr->name, curr->price);
		}
		fprintf(dest, "\n");
		curr = curr->next;
	}
  fprintf(dest, LINE_DIVIDER);
}

/* Frees contents of merchant including merchant itself */
void free_merchant(merchant_t *merchant) {
    free(merchant->email);
    free(merchant->password);
    free(merchant->folder_path);
    FREE_MENU(merchant->menu);
		if (merchant->input != stdin) {
			fclose(merchant->input);
		}
    free_unpaid_list(merchant->unpaid_orders);
    free(merchant);
}
