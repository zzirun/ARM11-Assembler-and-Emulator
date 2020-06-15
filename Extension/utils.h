#include "types.h"

#define MAX_MENU_ITEM_LENGTH (80)

#define INVALID_PAYMENT(type) (type < 0 || type > NUM_OF_PAYMENT_TYPES)

/* Lists all files in the directory specified by path */
void list_all_files(DIR *path);

/* Parses menu text file specified by path into an menu ADT */
bool parse_menu(char* path_to_menu, menu_t* menu);
/* Parses and adds a menu item into the menu */
bool parse_menu_item(char* item_str, menu_t* menu);

/* Adds specified quantity of order referenced by id in ascending order of id */
bool add_order(int id, int quantity, menu_t *menu, order_list_t *order_list);
/* Edits items in the list of orders */
void edit_order(order_list_t *order_list);

/* Mode 0 : prints menu; Mode 1 : prints list of orders */
void print_list(list_t *list, int mode);
#define PRINT_MENU(menu) print_list(menu, 0)
#define PRINT_ORDER_LIST(order_list) print_list(order_list, 1)

/* Makes a receipt, unpaid, given the orders and total price */
receipt_t *make_receipt(order_list_t *order_list);
/* Prints a receipt */
void print_receipt(receipt_t *receipt);