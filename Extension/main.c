#include "merchant.h"
#include "utils.h"
#include "login.c"

/**
 * 1) Login with id
 *    * id checked with a text file containing registered ids
 *      a) if a match is found, returns the path to the folder containing the merchant's data (menu text file) as a string
 *      b) else, unknown id, ask for registration of new merchants - involves setting a password, providing a menu 
 *         - then returns path to new folder created for the merchant 
 * 2) Merchant's email login authentication
 *    * asks for the merchant's email address and password to allow sending of receipts
 * 3) Parse the menu text file provided by merchant upon registration in the folder associated with the merchant
 *    into a linked list of menu items (fields: item id, name, price) sorted by their id (ascending)
 * 4) The merchant can take orders or exit the program 
 *    a) Taking orders
 *       * Items are added/changed/removed from the orders by specifying the item id and quantity of purchase
 *       * When the order list is finalised, a receipt is created and a payment method is chosen
 *       * After payment, the receipt is sent to the customer's email address
 *       * Return to step 4
 *    b) Input N/n upon asking for program continuation corresponds to program termination 
 */ 

#define TERMINATE(c) (c == 'N' || c == 'n')
#define CONTINUE(c) (c == 'Y' || c == 'y')

bool continue_program() {
  char termination;
  do {
    printf("Do you want to continue? [y/n] > ");
	  scanf(" %c", &termination);
  } while (!(TERMINATE(termination) || CONTINUE(termination)));
  return CONTINUE(termination);
}

void pay(receipt_t *receipt) {
  payment_t payment_type;
  do {
    printf("How do you want to pay? [0]Cash [1]Card [2]e-Wallet > ");
    scanf("%d", &payment_type);
  } while (INVALID_PAYMENT(payment_type));	
  receipt->payment_type = payment_type;
}

int main(void) {
  /* Attempt to login */
	char* folder_path = login();

  /* Merchant's email login authentication */
	printf("To allow us to send email receipts to customers,\n");
	printf("Please enter your email address > "); //Currently only support gmail accounts that allow access to less secure apps
	char email[MAX_EMAIL_LENGTH] = {0};
	scanf("%s", email);
	printf("Please enter you password > ");
	char password[MAX_PASSWORD_LENGTH] = {0};
	scanf(" %s", password);

  /* Make merchant's menu */
	menu_t* menu = MENU_NEW(); 
  char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
	snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", folder_path, menu_name);
  parse_menu(path_to_menu, menu);

  /* Taking orders */
  while (continue_program()) {
    /* Make new empty order list and take order, returning a receipt */
    order_list_t* order_list = ORDER_LIST_NEW();
    receipt_t * receipt = take_order(menu, order_list);
    /* Get payment type */
    pay(receipt);
    /* Store receipt in merchant's folder */
    char* path_to_receipt = store_receipt(folder_path, receipt);
    /* Send receipt to customer */
    printf("Please enter customer email address to receive receipt > ");
		char receiver[MAX_EMAIL_LENGTH] = {0};
		scanf(" %s", receiver);

		printf("Please input name of issuer > ");
		char issuer[MAX_ID_LENGTH];
		scanf(" %s", issuer);
		char subject[MAX_SUBJECT_LENGTH] = {0};
		snprintf(subject, MAX_SUBJECT_LENGTH, "%s%s", receipt_base, issuer);
		printf("Sending your receipt... \n");
		send_receipt(email, password, receiver, subject, path_to_receipt);

		printf("DONE!\n");
    FREE_ORDER_LIST(order_list);
		free(path_to_receipt);
  }

	FREE_MENU(menu);
	free(folder_path);
	printf("Thank you for using our service! :D \n");

	return EXIT_SUCCESS;
}