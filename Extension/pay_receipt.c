#include "merchant.h"

#define INVALID_PAYMENT(type) (type < 0 || type > NUM_OF_PAYMENT_TYPES)
#define MAX_RECEIPT_PATH_LENGTH (75)
#define MAX_SUBJECT_LENGTH (50)

receipt_t *make_receipt(unpaid_t *unpaid) {
  order_list_t *order_list = unpaid->order_list;
  // Create receipt and assign orders
  receipt_t *receipt = calloc(1, sizeof(receipt_t));
  if (!receipt) {
    perror("Receipt cannot be created");
    exit(EXIT_FAILURE);
  }
	receipt->order_list = order_list;
  // Calculate price
  order_t *curr = order_list->head->next;
  while (curr) {
    receipt->total_amount += curr->price * curr->quantity;
    curr = curr->next;
  }
  return receipt;
}

void print_receipt(receipt_t *receipt, FILE* dest) {
  fprintf(dest, "\n\t Your receipt : ");
  PRINT_ORDER_LIST(receipt->order_list, dest);
  char *final_output = "Your total amount due is      :";
	fprintf(dest, "\t %-34s %8.2f\n\n", final_output, receipt->total_amount);
  if (receipt->payment_type) {
    fprintf(dest, "\nYou paid by : %s\n", payment_string[receipt->payment_type - 1]);
  }
}

char* store_receipt(merchant_t *merchant, receipt_t *receipt) {
  /* Allocate path to receipt string */
  char* path_to_receipt = calloc(MAX_RECEIPT_PATH_LENGTH, sizeof(char));
  if (!path_to_receipt) {
    fprintf(stderr, "Cannot store receipt");
    // MAKE TERMINATE FUNCTION
  }
  /* Modify receipt path name to merchant's (folder) name and current time */
	char* curr_time = current_time();
  snprintf(path_to_receipt, MAX_RECEIPT_PATH_LENGTH, "%s%s",
    merchant->folder_path, curr_time);
	free(curr_time);
  /* Enter receipt data (order list, total, payment method) */
	FILE* dest = fopen(path_to_receipt, "w");
  print_receipt(receipt, dest);
	fclose(dest);
	return path_to_receipt;
}

/* Calls Python function to authenticate email on SMTP server */
bool connect(char* email, char* password) {
	bool result = true;
	char command[256] = {0};
	snprintf(command, sizeof(command), "python3 ./send_email.py %s %s", email, password);
	printf("Trying to authenticate...\n");
	FILE* fp = popen(command, "r");
	char buffer[256] = {0};
	fgets(buffer, 256, fp);
	if (!strcmp(buffer, "Unable to authenticate\n")) {
		result = false;
	}
	pclose(fp);
	return result;
}

/* Calls Python function to send receipt as email */
void send_receipt(merchant_t *merchant, char* path_to_receipt) {
  /* Get customer email */
  char receiver[MAX_EMAIL_LENGTH] = {0};
  printf("Please enter customer email address to receive receipt > ");
  fscanf(merchant->input, "%s", receiver);
  /* Get name of issuer which is printed in the email description */
  char issuer[MAX_ID_LENGTH];
  printf("Please input name of issuer > ");
  fscanf(merchant->input, " %s", issuer);
  char subject[MAX_SUBJECT_LENGTH] = {0};
  snprintf(subject, MAX_SUBJECT_LENGTH, "%s%s", receipt_base, issuer);
  /* Send receipt */
  printf("Sending your receipt... \n");
	char args[256] = {0};
	snprintf(args, sizeof(args), "python3 ./send_email.py %s %s %s %s %s",
  merchant->email, merchant->password, receiver, subject, path_to_receipt);
	system(args);
  /* Done!! */
  printf("DONE!\n");
}

/* Returns the current time as a string, used as unique identifier for receipts */
char* current_time(void) {
	time_t rawtime;
	time(&rawtime);
  struct tm* timeinfo = localtime (&rawtime);
  char* time_str = calloc(20, sizeof(char));
	snprintf(time_str, 20, "%02d:%02d:%02d_%02d%02d%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
  return time_str;
}

// DRAFT DONE, TO CHECK
void pay(merchant_t *merchant, unpaid_t *unpaid_in) {
  // Pay now or pay later (input NULL)
  unpaid_t *unpaid = unpaid_in ? unpaid_in : get_unpaid_order(merchant);
  // Check unpaid exists
  if (!unpaid) {
    return;
  }
  // Create receipt, calculates total and print
  receipt_t *receipt = make_receipt(unpaid);
  print_receipt(receipt, merchant->output);
  // Assign payment type
  int payment_type;
  do {
    fprintf(merchant->output, "How do you want to pay? [1]Cash [2]Card [3]e-Wallet > ");
    fscanf(merchant->input,"%d", &payment_type);
  } while (INVALID_PAYMENT(payment_type));
  receipt->payment_type = payment_type;
  //Store receipt in merchant's folder
  char* path_to_receipt = store_receipt(merchant, receipt);
  //Send receipt to customer
  send_receipt(merchant, path_to_receipt);
  //Remove unpaid order from list, free memory
  remove_unpaid_order(merchant, unpaid);
  free(receipt);
  free(path_to_receipt);
}

/*
void pay(merchant_t *merchant, order_list_t *ol) {
  unpaid_t *unpaid = get_unpaid_order(merchant);
	if (!unpaid) {
		return;
	}
	order_list_t *order_list = unpaid->order_list;
  receipt_t *receipt = make_receipt(order_list);
  print_receipt(receipt, merchant->output);
  int payment_type;
  do {
    fprintf(merchant->output, "How do you want to pay? [1]Cash [2]Card [3]e-Wallet > ");
  } while (!fscanf(merchant->input,"%d", &payment_type)
            || INVALID_PAYMENT(payment_type));
  receipt->payment_type = payment_type;
  // Store receipt in merchant's folder
  char* path_to_receipt = store_receipt(merchant, receipt);
  // Send receipt to customer
  send_receipt(merchant, path_to_receipt);
  // Remove unpaid order from list, free memory
  remove_unpaid_order(merchant, unpaid);
	//Order_list freed by remove_unpaid_order
	free(receipt);
  free(path_to_receipt);
}
*/
