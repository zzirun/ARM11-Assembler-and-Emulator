#include "types.h"
#define MAX_ID_LENGTH (20)
#define MAX_EMAIL_LENGTH (30) 
/*****************************************************************************/
// UTILS :
/* Initialises constants shared across files */
void init(void);

/* Parses menu text file specified by path into an menu ADT */
bool parse_menu(char *path_to_menu, menu_t* menu);
/* Parses and adds a menu item into the menu */
bool parse_menu_item(char *item_str, menu_t* menu);

/* Mode 0 : prints menu; Mode 1 : prints list of orders */
void print_list(list_t *list, int mode, FILE *dest);
#define PRINT_MENU(menu, dest) print_list(menu, 0, dest)
#define PRINT_ORDER_LIST(order_list, dest) print_list(order_list, 1, dest)

void free_merchant(merchant_t *merchant);

/*****************************************************************************/
// UNPAID_UTILS :
/* Makes new, empty list of unpaid orders */
unpaid_list_t *unpaid_list_new(void);
/* Makes empty unpaid with empty order and no customer name associated */
unpaid_t *create_unpaid(void);
/* Adds to the back of the merchant's list of unpaid orders */
void add_unpaid(merchant_t *merchant, unpaid_t *unpaid);
/* Gets an unpaid order from the list */
unpaid_t *get_unpaid_order(merchant_t *merchant);
/* Removes the specified unpaid order from the list */
void remove_unpaid_order(merchant_t *merchant, unpaid_t *unpaid);
void free_unpaid_list(unpaid_list_t *unpaid_list);

/*****************************************************************************/
// ORDER :
/* Adds specified quantity of order referenced by id in ascending id order */
bool add_order(int id, int quantity, menu_t *menu, order_list_t *order_list);

/*****************************************************************************/
// PAY_RECEIPT :
/* Gets current time, used as receipt name */
char *current_time(void);
/* Makes a unpaid receipt, calculates total price from the given order */
receipt_t *make_receipt(unpaid_t *unpaid);
/* Prints a receipt - order, price and payment method if paid */
void print_receipt(receipt_t *receipt, FILE *dest);
/* Stores receipt in merchant folder */
char *store_receipt(merchant_t *merchant, receipt_t *receipt);
/* To authenticate email */
bool connect(char *email, char *password);
/* Sends receipt stored in path to customer */
void send_receipt(merchant_t *merchant, char *path_to_receipt);


/*****************************************************************************/
// LOGIN
/* Registers new IDs - stores ID and its password hash in the ID file */
char *register_new(FILE *id_file, char *id);
/* Adds a pepper to a password upon registration */
char *pepper(char *password);
/* For password hashing */
long hash(char *to_hash);

/* Prompts merchant login or new merchant registration */
char *login(FILE *f);
/* Checks if ID is registered and retrieves corresponding password hash */
bool check_id(FILE *id_file, char *id, long *hash_pw, FILE *input_src);
/* Checks entered password against the hash retrieved from a registered ID */
bool check_password(char *pw, long hash_pw);

/*****************************************************************************/







