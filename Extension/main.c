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

int main(int argc, char **argv) {
  init();
  FILE *fp = NULL;
  // Interactive Mode
  if(argc == 1) {
      fp = stdin;
  }
  // Testing Mode (Passing in a text file of inputs to test)
  else if(argc == 2) {
      fp = fopen(argv[1], "r");
      if(!fp){
          perror("error opening file");
          exit(EXIT_FAILURE);
      }
  }
  else {
      perror("Wrong number of arguments passed");
      exit(EXIT_FAILURE);
  }

  merchant_t *merchant = login_and_init(fp);

  while (1) {
    int action;
    printf("\nChoose an action : [0]Quit [1]Take Order [2]Edit Order [3]Cancel Order [4]Pay Order > ");
		fscanf(fp, "%d", &action);
		while (action < 0 || action > 4) {
			printf("Invalid input! Please choose an action : [0]Quit [1]Take Order [2]Edit Order [3]Cancel Order [4]Pay Order [5]Load New merchant > ");
			getchar(); //Absorbs new line
			fscanf(fp, "%d", &action);
		}
    if (action == QUIT) {
			char response;
			printf("Do you want to login to another user? [y/n] > ");
			fscanf(fp, " %c", &response);
			while (response != 'y' && response != 'Y' && response != 'n' && response != 'N') {
				printf("Invalid input! Do you want to login to another user? [y/n] > ");
				fscanf(fp, " %c", &response);
			}
			if (response == 'y' || response == 'Y') {
				printf("\n");
				free_merchant(merchant);
				merchant = login_and_init(fp);
				continue;
			} else {
				break;
			}
    }
    switch(action) {
      case TAKE_ORDER:
        take_order(merchant, NULL, NULL);
        break;
      case EDIT_ORDER:
        edit_order(merchant);
        break;
      case CANCEL_ORDER:
      	cancel_order(merchant);
        break;
      case PAY_ORDER:
        pay(merchant, NULL);
        break;
      default:
        assert(0);
        break;
    }
  }

  free_merchant(merchant);

	printf("\nThank you for using our service! :D \n");

	return EXIT_SUCCESS;
}
