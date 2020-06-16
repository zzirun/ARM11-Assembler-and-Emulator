#include "types.h"

#define PERMISSION_BITS (0777)
#define MAX_ID_LENGTH (20)
/* Max length of email for the sender and receiver of a receipt */
#define MAX_EMAIL_LENGTH (30)
#define MAX_FOLDER_PATH_LENGTH (30) //strlen("Merchants/") + strlen(user_id)
#define MAX_MENU_PATH_LENGTH (35) //strlen(folder_path) + strlen("menu.txt")
#define MAX_SUBJECT_LENGTH (50)
#define MAX_PASSWORD_LENGTH (50)
#define MAX_RECEIPT_PATH_LENGTH (75)
#define MAX_CUSTOMER_NAME_LENGTH (10)


#define MAX_MENU_ITEM_LENGTH (80)

#define INVALID_PAYMENT(type) (type < 0 || type > NUM_OF_PAYMENT_TYPES)

/* Lists all files in the directory specified by path */
void list_all_files(DIR *path);

char* current_time(void);

/* Parses menu text file specified by path into an menu ADT */
bool parse_menu(char* path_to_menu, menu_t* menu);
/* Parses and adds a menu item into the menu */
bool parse_menu_item(char* item_str, menu_t* menu);

/* Adds specified quantity of order referenced by id in ascending order of id */
bool add_order(int id, int quantity, menu_t *menu, order_list_t *order_list);

/* Mode 0 : prints menu; Mode 1 : prints list of orders */
void print_list(list_t *list, int mode, FILE *dest);
#define PRINT_MENU(menu, dest) print_list(menu, 0, dest)
#define PRINT_ORDER_LIST(order_list, dest) print_list(order_list, 1, dest)

/* Makes a receipt, unpaid, given the orders and total price */
receipt_t *make_receipt(order_list_t *order_list);
/* Prints a receipt */
void print_receipt(receipt_t *receipt, FILE *dest);
/* Stores receipt in merchant folder */
char* store_receipt(merchant_t *merchant, receipt_t *receipt);
/* Sends receipt stored in path to customer */
void send_receipt(merchant_t *merchant, char* path_to_receipt);

/* Gets unpaid order from list, given input */
unpaid_t *get_unpaid_order(merchant_t *merchant);
/* Removes unpaid order from list */
void remove_unpaid_order(merchant_t *merchant, unpaid_t *unpaid);
/* Adds unpaid order to back of list */
void add_unpaid(merchant_t *merchant, order_list_t *order_list);
/* Makes new unpaid list */
unpaid_list_t *unpaid_list_new(void);
/* Frees unpaid list */
void free_unpaid_list(unpaid_list_t *unpaid_list);