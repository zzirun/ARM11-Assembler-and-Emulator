#include "merchant.h"

#define CHOOSE_ACTION_PROMPT \
("\nChoose an action : [0]Quit [1]Take Order [2]Edit Order [3]Cancel Order [4]Pay Order > ")

#define INVALID_ACTION(action) (action < 0 || action >= NUM_ACTION)

int main(int argc, char **argv) {
  merchant_t *merchant;

  if(argc == 0) {
    // Interactive Mode
    merchant = login_and_init(NULL, NULL);
  } else if (argc == 1) {
    // Testing Mode (Passing in a text file of inputs to test)
    merchant = login_and_init(argv[1], NULL);
  } else if (argc == 2) {
    merchant = login_and_init(argv[1], argv[2]);
  } else {
    fprintf(stderr, "Wrong number of arguments passed");
    exit(EXIT_FAILURE);
  }


  while (1) {
    int action;
    printf(CHOOSE_ACTION_PROMPT);
		fscanf(merchant->input, "%d", &action);
		while (INVALID_ACTION(action)) {
			printf(CHOOSE_ACTION_PROMPT);
			getchar(); //Absorbs new line
			fscanf(merchant->input, "%d", &action);
		}
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
