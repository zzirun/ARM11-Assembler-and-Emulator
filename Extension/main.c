#include "merchant.h"

/** MODIFY COMMENT
 * 1) Login with id
 *    * id checked with a text file containing registered ids
 *      a) if a match is found, returns the path to the folder containing the merchant's data (merchant text file) as a string
 *      b) else, unknown id, ask for registration of new merchants - involves setting a password, providing a merchant 
 *         - then returns path to new folder created for the merchant 
 * 2) Merchant's email login authentication
 *    * asks for the merchant's email address and password to allow sending of receipts
 * 3) Parse the merchant text file provided by merchant upon registration in the folder associated with the merchant
 *    into a linked list of merchant items (fields: item id, name, price) sorted by their id (ascending)
 * 4) The merchant can take orders or exit the program 
 *    a) Taking orders
 *       * Items are added/changed/removed from the orders by specifying the item id and quantity of purchase
 *       * When the order list is finalised, a receipt is created and a payment method is chosen
 *       * After payment, the receipt is sent to the customer's email address
 *       * Return to step 4
 *    b) Input N/n upon asking for program continuation corresponds to program termination 
 */ 



int main(void) {

  merchant_t *merchant = login_and_init();

  while (1) {
    action_t action;
    printf("Choose Action: [0]Quit [1]Take Order [2]Edit Order [3]Cancel Order [4]Pay Order [5]Load New merchant");
    // ADD DO WHILE FOR CHECKING ACTION INPUT
    // CHECK IF ERROR IF ACTION_T TAKES IN A INVALID NUMBER
    scanf("%d", &action);
    if (action == QUIT) {
      break;
    }
    switch(action) {
      case TAKE_ORDER:
        take_order(merchant, NULL);
        break;
      case EDIT_ORDER:
        edit_order(merchant);
        break;
      case CANCEL_ORDER:
        // TO DO cancel_order();
        break;
      case PAY_ORDER:
        pay(merchant, NULL);
        break;
      case LOAD_NEW_MENU:
        // Optional TO DO Make load function 
        break;
      default:
        assert(0);
        break;
    }
  } 

  free_merchant(merchant);

	printf("Thank you for using our service! :D \n");

	return EXIT_SUCCESS;
}