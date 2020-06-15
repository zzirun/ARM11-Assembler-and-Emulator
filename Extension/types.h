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

typedef enum login_t {
	USERS,
	MERCHANTS
} login_t;

char *login_data_t[2] = {"userID.txt", "merchantID.txt"};

char *login_folder_t[2] = {"Users/", "Merchants/"};

char *payment_string[3] = {"Cash","Credit/Debit Card", "e-Wallet"};

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

typedef struct list_t {
	list_elem_t *head;
	list_elem_t *tail;
} list_t, menu_t, order_list_t;


typedef enum payment_t {
  CASH = 0,
  CARD = 1,
  E_WALLET = 2
} payment_t;

#define NUM_OF_PAYMENT_TYPES (2)
#define INVALID_PAYMENT(type) (type < 0 || type > NUM_OF_PAYMENT_TYPES)

typedef struct receipt_t {
	order_list_t *order_list;
	float total_amount;
	payment_t payment_type;
} receipt_t;

list_elem_t *list_elem_new(void);
#define MENU_ITEM_NEW() list_elem_new()
#define ORDER_NEW() list_elem_new()

list_t *list_new(void);
#define MENU_NEW() list_new()
#define ORDER_LIST_NEW() list_new()

bool add_menu_item(char* item_str, menu_t* menu);

void free_list_elem(list_elem_t* item);
#define FREE_MENU_ITEM(menu_item) free_list_elem(menu_item)
#define FREE_ORDER(order) free_list_elem(order)

void free_list(list_t *list);
#define FREE_MENU(menu) free_list(menu)
#define FREE_ORDER_LIST(order_list) free_list(order_list)

order_t *order_item(menu_t *menu, int id);

void add_order(order_t *order, order_list_t *order_list);

void print_list(list_t *list, int mode);
#define PRINT_MENU(menu) print_list(menu, 0)
#define PRINT_ORDER_LIST(order_list) print_list(order_list, 1)

void list_all_files(DIR *path);

receipt_t *make_receipt(order_list_t *order_list, float total);
void print_receipt(receipt_t *receipt);

#endif //EXTENSION_TYPES
