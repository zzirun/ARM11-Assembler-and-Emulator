#ifndef EXTENSION_TYPES
#define EXTENSION_TYPES

typedef enum login_type {
	USERS,
	MERCHANTS
} login_type_t;

char* login_data_t[2] = {"userID.txt", "merchantID.txt"};

char* login_folder_t[2] = {"Users/", "Merchants/"};

char* payment_string[3] = {"Cash","Credit/Debit Card", "e-Wallet"};

typedef struct list_elem {
	struct list_elem* next;
	char* name;
	float price;
	int id;
	/*Quantity only for customer order use, default to 0 in menu. I think we can talk about
		using this for recording stock availability as a possible optimisation of our
		extension in the final report. I guess right now we can just not use it in
		the menu cuz we're storing everything as plain text and it'd be hard to update
		without a database.*/
	int quantity;
} list_elem_t;

typedef struct list {
	list_elem_t* head;
	list_elem_t* tail;
} list_t;

typedef struct receipt {
	list_t* order_list;
	float total_amount;
	int payment_type;
} receipt_t;

list_elem_t* list_elem_new(void);
list_t* list_new(void);
void add_to_list(list_elem_t* item, list_t* list);
void free_list(list_t* list);
list_elem_t* clone_item(list_t* menu, int id);
void add_order(list_elem_t* item, list_t* list);
void print_list(list_t* list, int mode);

#endif //EXTENSION_TYPES
