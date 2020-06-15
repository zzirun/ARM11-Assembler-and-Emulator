#ifndef EXTENSION_TYPES
#define EXTENSION_TYPES

#define PERMISSION_BITS (0777)
#define MAX_ID_LENGTH (20)
#define MAX_EMAIL_LENGTH (30)
#define MAX_FOLDER_PATH_LENGTH (30) //strlen("Merchants/") + strlen(user_id)
#define MAX_MENU_PATH_LENGTH (35) //strlen(folder_path) + strlen("menu.txt")
#define MAX_SUBJECT_LENGTH (50)
#define MAX_PASSWORD_LENGTH (50)
#define MAX_RECEIPT_PATH_LENGTH (75)

char* id_data = "merchantID.txt";

char* base_login_folder = "Merchants/";

char* menu_name = "menu.txt";

char* payment_string[3] = {"Cash","Credit/Debit Card", "e-Wallet"};

char* receipt_base = "Receipt_from_";

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

list_elem_t* list_elem_new(void);
list_t* list_new(void);
void add_to_list(list_elem_t* item, list_t* list);
void free_list(list_t* list);
list_elem_t* clone_item(list_t* menu, int id);
void add_order(list_elem_t* item, list_t* list);
void print_list(list_t* list, int mode);

#endif //EXTENSION_TYPES
