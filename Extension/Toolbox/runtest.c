#include "../merchant.h"

#define MAX_RESULT_LENGTH (100)

char* password = "THisISaTesTPasSWorD!";

char* registered_user_txt = "test_BRANDON/example_id_1.txt";

char* non_registered_user_txt = "test_BRANDON/example_id_2.txt";

char* password_1 = "bhsz"; //Correct password for user in registered_user_txt

char* password_2 = "chsz";

char* path_to_menu = "test_BRANDON/test_menu.txt";

char* test_result[2] = {"Failed", "Passed"};

/* Checks if a password has a random characted appended after pepper */
void test_pepper(FILE* test_log) {
	int outcome = 1;
	char* pepper_pw = pepper(password);
	if (strlen(pepper_pw) != strlen(password) + 1) {
		outcome = 0;
	} else {
		for (int i = 0; i < strlen(password); i++) {
			if (pepper_pw[i] != password[i]) {
				outcome = 0;
				break;
			}
		}
		if (((int) pepper_pw[strlen(password)] < 33) || ((int) pepper_pw[strlen(password)] > 122)) {
			outcome = 0;
		}
	}
	free(pepper_pw);
	fprintf(test_log, "PEPPER TEST : %s\n", test_result[outcome]);
}

void test_hash(FILE* test_log) {
	int outcome = 1;
	char* pepper_1 = pepper(password_1);
	char* pepper_2 = pepper(password_2);
	if (hash(pepper_1) == hash(pepper_2)) {
		outcome = 0;
	}
	free(pepper_1);
	free(pepper_2);
	fprintf(test_log, "HASH TEST : %s\n", test_result[outcome]);
}

void test_check_id(FILE* test_log) {
	int outcome = 1;
	FILE* user_data = fopen("merchantID.txt", "r");
	FILE* registered = fopen(registered_user_txt, "r");
	FILE* not_registered = fopen(non_registered_user_txt, "r");
	char user_id[MAX_ID_LENGTH] = {0};
	long password = 0;
	if (!check_id(user_data, user_id, &password, registered)) {
		outcome = 0;
	}
	if (check_id(user_data, user_id, &password, not_registered)) {
		outcome = 0;
	}
	fclose(user_data);
	fclose(registered);
	fclose(not_registered);
	fprintf(test_log, "CHECK ID TEST : %s\n", test_result[outcome]);
}

void test_check_password(FILE* test_log) {
	int outcome = 1;
	FILE* user_data = fopen("merchantID.txt", "r");
	FILE* registered = fopen(registered_user_txt, "r");
	char user_id[MAX_ID_LENGTH] = {0};
	long password = 0;
	if (!check_id(user_data, user_id, &password, registered)) {
		outcome = 0;
	}
	if (!check_password(password_1, password)) {
		outcome = 0;
	}
	fclose(user_data);
	fclose(registered);
	fprintf(test_log, "CHECK PASSWORD TEST : %s\n", test_result[outcome]);
}

void test_login(FILE* test_log) {
	int outcome = 1;
	FILE* test_login = fopen("test_BRANDON/test_login.txt", "r");
	char* folder_path = login(test_login);
	if (strcmp(folder_path, "Merchants/bhsz/")) {
		outcome = 0;
	}
	free(folder_path);
	fclose(test_login);
	fprintf(test_log, "LOGIN TEST : %s\n", test_result[outcome]);
}

void test_parse_menu(FILE* test_log) {
	int outcome = 1;
	menu_t* menu = MENU_NEW();
	if (parse_menu(path_to_menu, menu)) {
		menu_item_t* curr = menu->head->next;
		while (curr) {
			if (!curr->name || curr->id < 0 || curr->price < 0) {
				outcome = 0;
			}
			curr = curr->next;
		}
	} else {
		outcome = 0;
	}
	FREE_MENU(menu);
	fprintf(test_log, "PARSE MENU TEST : %s\n", test_result[outcome]);
}

void test_add_order(FILE* test_log) {
	int outcome = 1;
	menu_t* menu = MENU_NEW();
	order_list_t* order = ORDER_LIST_NEW();
	parse_menu(path_to_menu, menu);
	add_order(1, 1, menu, order);
	order_t* ptr = order->head->next;
	add_order(1, 5, menu, order);
	if (ptr->quantity != 5) {
		outcome = 0;
	}
	add_order(5,1, menu, order);
	if (ptr->next->id != 5) {
		outcome = 0;
	}
	add_order(2, 3, menu, order);
	if (ptr->next->id != 2 || ptr->next->quantity != 3) {
		outcome = 0;
	}
	FREE_MENU(menu);
	FREE_ORDER_LIST(order);
	fprintf(test_log, "ADD ORDER TEST : %s\n", test_result[outcome]);
}

int main(void) {
	init();
	FILE* test_log = fopen("test_BRANDON/test.log", "w");
	test_pepper(test_log);
	test_hash(test_log);
	test_check_id(test_log);
	test_check_password(test_log);
	test_login(test_log);
	test_parse_menu(test_log);
	test_add_order(test_log);
	fclose(test_log);
	return EXIT_SUCCESS;
}
