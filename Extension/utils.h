#include "types.h"

#define INVALID_PAYMENT(type) (type < 0 || type > NUM_OF_PAYMENT_TYPES)
//#define MAX_USER_ID_LENGTH (50)

#define INVALID_ITEM_ID(item_id, list) \
(item_id > list->tail->id || item_id < list->head->next->id)

#define FINALIZE_ORDER(input) \
(!strcmp(input, "end") || !strcmp(input, "END"))

#define EDIT_ORDER(input) \
(!strcmp(first_input, "e") || !strcmp(first_input, "E"))

/**************************************************************************************/
// UTILS :
/* Initialises constants */
void init(void);
void free_merchant(merchant_t *merchant);
/* Parses menu text file specified by path into an menu ADT */
bool parse_menu(char* path_to_menu, menu_t* menu);
/* Parses and adds a menu item into the menu */
bool parse_menu_item(char* item_str, menu_t* menu);
/* Mode 0 : prints menu; Mode 1 : prints list of orders */
void print_list(list_t *list, int mode, FILE *dest);
#define PRINT_MENU(menu, dest) print_list(menu, 0, dest)
#define PRINT_ORDER_LIST(order_list, dest) print_list(order_list, 1, dest)
/**************************************************************************************/
// UNPAID UTILS:
unpaid_t *create_unpaid(void);
/* Gets unpaid order from list, given input */
unpaid_t *get_unpaid_order(merchant_t *merchant);
/* Removes unpaid order from list */
void remove_unpaid_order(merchant_t *merchant, unpaid_t *unpaid);
/* Adds unpaid order to back of list */
void add_unpaid(merchant_t *merchant, unpaid_t *unpaid);
/* Makes new unpaid list */
unpaid_list_t *unpaid_list_new(void);
/* Frees unpaid list */
void free_unpaid_list(unpaid_list_t *unpaid_list);
/**************************************************************************************/
// ORDER:
/* Adds specified quantity of order referenced by id in ascending order of id */
bool add_order(int id, int quantity, menu_t *menu, order_list_t *order_list);

/**************************************************************************************/
// PAY RECEIPT
/* Makes a receipt, unpaid, given the orders and total price */
receipt_t *make_receipt(unpaid_t *unpaid);
/* Prints a receipt */
void print_receipt(receipt_t *receipt, FILE *dest);
/* Stores receipt in merchant folder */
char* store_receipt(merchant_t *merchant, receipt_t *receipt);
/* To authenticate email */
bool connect(char* email, char* password);
/* Sends receipt stored in path to customer */
void send_receipt(merchant_t *merchant, char* path_to_receipt);
void free_receipt(receipt_t *receipt);
/* Gets current time, we use this as receipt name */
char* current_time(void);
/* Pays an order */

/**************************************************************************************/
// Login
char* pepper(char* str);

long hash(char* to_hash);

bool check_password(char* pw, long hash_pw);

bool check_id(FILE* fp, char* id, long* password, FILE *f);

char* register_new(FILE* fp, char* id);

char* login(FILE *f);







