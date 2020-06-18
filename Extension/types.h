#ifndef EXTENSION_TYPES_H
#define EXTENSION_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>

extern char *id_data;

extern char *base_login_folder;

extern char *menu_name;

extern char *payment_string[3];

extern char *receipt_base;

#define NUM_ACTION (5)
#define NUM_OF_PAYMENT_TYPES (3)

/* Actions that can be carried out by the system */
typedef enum action_t {
  QUIT = 0,
  TAKE_ORDER = 1,
  EDIT_ORDER = 2,
  CANCEL_ORDER = 3,
  PAY_ORDER = 4,
} action_t;

/* Payment types */
typedef enum payment_t {
  UNPAID = 0,
  CASH = 1,
  CARD = 2,
  E_WALLET = 3
} payment_t;

/* Linked list element type - represents menu items or orders */
typedef struct list_elem_t {
	struct list_elem_t *next;
	char *name;
	float price;
	int id;
	int quantity;
    // Quantity only for customer order use, defaults to 0 in menu.
} list_elem_t, menu_item_t, order_t;

/* Linked list type - represents a menu or a list of orders */
typedef struct list_t {
	list_elem_t *head;
	list_elem_t *tail;
} list_t, menu_t, order_list_t;

/* Type to demarcate unpaid lists of orders */
typedef struct unpaid_t {
  order_list_t *order_list;
  char *customer_name;
  struct unpaid_t *prev;
  struct unpaid_t *next;
} unpaid_t;

/* List of unpaid lists of orders */
typedef struct unpaid_list_t {
  unpaid_t *head;
  unpaid_t *tail;
  int no_of_unpaid;
} unpaid_list_t;

/* Receipt with a customer's orders, total price, and payment type */
typedef struct receipt_t {
	order_list_t *order_list;
	float total_amount;
	payment_t payment_type;
} receipt_t;

/* Merchant data */
typedef struct merchant_t {
  char *email;
  char *password;
  char *folder_path;
  FILE *input;
  FILE *output;
  menu_t *menu;
  unpaid_list_t *unpaid_orders;
} merchant_t;


/* Creates new list element */
list_elem_t *list_elem_new(void);
#define MENU_ITEM_NEW() list_elem_new()
#define ORDER_NEW() list_elem_new()

/* Creates new list */
list_t *list_new(void);
#define MENU_NEW() list_new()
#define ORDER_LIST_NEW() list_new()

/* Frees a list element */
void free_list_elem(list_elem_t *item);
#define FREE_MENU_ITEM(menu_item) free_list_elem(menu_item)
#define FREE_ORDER(order) free_list_elem(order)

/*/ Frees a list */
void free_list(list_t *list);
#define FREE_MENU(menu) free_list(menu)
#define FREE_ORDER_LIST(order_list) free_list(order_list)

/* Adds item to end of a list */
void add_to_list(list_elem_t *elem, list_t *list);
#define ADD_MENU_ITEM(menu_item, menu) add_to_list(menu_item, menu)
#define ADD_ORDER(order, order_list) add_to_list(order, order_list)

#endif EXTENSION_TYPES_H
