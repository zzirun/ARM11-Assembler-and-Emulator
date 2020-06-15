#ifndef EXTENSION_TYPES_H
#define EXTENSION_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_OF_PAYMENT_TYPES (2)

char* id_data = "merchantID.txt";

char* base_login_folder = "Merchants/";

char* menu_name = "menu.txt";

char* payment_string[3] = {"Cash","Credit/Debit Card", "e-Wallet"};

char* receipt_base = "Receipt_from_";


/* Payment types */
typedef enum payment_t {
  CASH = 0,
  CARD = 1,
  E_WALLET = 2
} payment_t;

/* Login as customers or merchants */
typedef enum login_t {
	CUSTOMER,
	MERCHANTS
} login_t;

/* Linked list element type - represents menu items or orders */
typedef struct list_elem_t {
	struct list_elem_t *next;
	char *name;
	float price;
	int id;
	/*Quantity only for customer order use, default to 0 in menu. I think we can talk about
		using this for recording stock availability as a possible optimisation of our
		extension in the final report. I guess right now we can just not use it in
		the menu cuz we're storing everything as plain text and it'd be hard to update
		without a database.*/
	int quantity;
} list_elem_t, menu_item_t, order_t;

/* Linked list type - represents a menu or a list of orders */
typedef struct list_t {
	list_elem_t *head;
	list_elem_t *tail;
} list_t, menu_t, order_list_t;

/* Receipt with a customer's orders, total price, and payment type */
typedef struct receipt_t {
	order_list_t *order_list;
	float total_amount;
	payment_t payment_type;
} receipt_t;

/* Creates new list element */
list_elem_t *list_elem_new(void);
#define MENU_ITEM_NEW() list_elem_new()
#define ORDER_NEW() list_elem_new()

/* Creates new list */
list_t *list_new(void);
#define MENU_NEW() list_new()
#define ORDER_LIST_NEW() list_new()

/* Frees a list element */
void free_list_elem(list_elem_t* item);
#define FREE_MENU_ITEM(menu_item) free_list_elem(menu_item)
#define FREE_ORDER(order) free_list_elem(order)

/* Fress a list */
void free_list(list_t *list);
#define FREE_MENU(menu) free_list(menu)
#define FREE_ORDER_LIST(order_list) free_list(order_list)

/* Adds item to end of a list */
void add_to_list(list_elem_t *elem, list_t *list);
#define ADD_MENU_ITEM(menu_item, menu) add_to_list(menu_item, menu)
#define ADD_ORDER(order, order_list) add_to_list(order, order_list)

#endif //EXTENSION_TYPES
