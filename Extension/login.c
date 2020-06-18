#include "merchant.h"

#define PERMISSION_BITS (0777)
#define MAX_PASSWORD_LENGTH (50)
#define MAX_FOLDER_PATH_LENGTH (30) //strlen("Merchants/") + strlen(user_id)
#define MAX_MENU_PATH_LENGTH (35) //strlen(folder_path) + strlen("menu.txt")
#define ID_DATA_FILE ("merchantID.txt")
#define BASE_LOGIN_FOLDER ("Merchants/")

/** Peppering a password to improve security : 
 *  Appends a random ASCII character between 33 ('!') and 122 ('z')
 */
char* pepper(char* password) {
  // Seed with time
	srand(time(NULL)); 
  // Size of original string including the sentinel character
	int size = strlen(password) + 1; 
  // Make room for pepper
	char* password_copy = calloc(size + 1, sizeof(char)); 
	strcpy(password_copy, password);
  // Set pepper
	password_copy[size - 1] = rand() % 89 + 33; 
	return password_copy;
}

/** djb2 Hashing of peppered password :
 *  Uses a magic constant which produces better avalanching 
 *  - changing the input slightly causes massive change in output
 */
long hash(char* to_hash) {
	long hash = 5381;
  while (*to_hash) {
		hash = ((hash << 5) + hash) + *to_hash;
		to_hash++;
	}
  return hash;
}

/* Returns true if the password after hashing matches the password hash */
bool check_password(char* pw, long hash_pw) {
  // Copy password input in place
	char pw_copy[strlen(pw) + 2];
	pw_copy[sizeof(pw_copy) - 1] = 0;
	strcpy(pw_copy, pw);
	// Brute force attempt to check the pepper appended upon registration
	for (int i = 33; i < 123; i++) {
		pw_copy[sizeof(pw_copy) - 2] = i;
		// Check hash of peppered password against given hash
    if (hash(pw_copy) == hash_pw) {
			return true;
		}
	}
	return false;
}

/** Checks if an ID is registered
 *  against id_file containing all registered IDs
 *  If registered, the password hash stored in the same location
 *  will be retrieved into the pw_hash and return true. 
 *  If ID is not found, return false.
 */
bool check_id(FILE *id_file, char *id, long *pw_hash, FILE *input_src) {
  if(input_src == stdin) {
		printf("Please enter you user ID to login > ");
  }
	fscanf(input_src,"%s", id);
	char str[MAX_ID_LENGTH] = {0};
	bool registered = false;
  // Split at space
	const char delim[2] = " "; 
	while (fgets(str, MAX_ID_LENGTH + MAX_PASSWORD_LENGTH, id_file)) {
		//Remove new line
    str[strlen(str) - 1] = 0; 
		char* token = strtok(str, " \n");
		// Check for empty string
    if (!token) { 
			break;
		}
		if (!strcmp(token, id)) {
			registered = true;
			token = strtok(NULL, delim);
			*pw_hash = atol(token); 
			break;
		}
	}
	return registered;
}

/** Register new id, requires merchant to provide a password and menu.
 *  The id is stored in the id_file with the peppered and hashed password.
 *  Returns the path to a folder newly created which corresponds to the id.
 */
char* register_new(FILE* id_file, char* id) {
	printf("Please enter a password > ");
	char pw[MAX_PASSWORD_LENGTH] = {0};
	scanf("%s", pw);
	char* path_name = calloc(MAX_FOLDER_PATH_LENGTH, sizeof(char));
	snprintf(path_name, MAX_FOLDER_PATH_LENGTH, "%s%s/", BASE_LOGIN_FOLDER, id);
	if (!mkdir(path_name, PERMISSION_BITS)) {
		printf("Thank you for joining us as a merchant! Please drag and drop your menu.txt into Merchants/%s\n" ,id);
		printf("Press any key when you have done so to continue > ");
		// To skip the \n from previous input
    getchar(); 
		getchar();
		char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
		snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", path_name, "menu.txt");
		while (!fopen(path_to_menu, "r")) {
			printf("menu.txt not found! Please make sure it is at the right location and named correctly.\n");
			printf("Press any key when you have done so to continue > ");
			getchar();
		}
		char* pw_pepper = pepper(pw); 
		fprintf(id_file, "%s", id);
    //Add space between user ID and password
		fprintf(id_file, " "); 
		fprintf(id_file, "%ld\n", hash(pw_pepper));
		free(pw_pepper);
		return path_name;
	} else {
		perror("Failed to create folder");
		exit(EXIT_FAILURE);
	}
}

/** User is prompted for an ID checked against a file of registered ids.
 *  If user has registered, he/she will be prompted for a password. 
 *  The password is checked to allow for login.
 *  If user has not registered, he/she will be prompted to register.
 *  A successful login/register returns the path 
 *  to the folder corresponding to the id.
 */
char* login(FILE *f) {
	char* result = calloc(MAX_FOLDER_PATH_LENGTH, sizeof(char));
  // Open for reading and possibly writing
	FILE* fp = fopen(ID_DATA_FILE, "r+"); 
	char id[MAX_ID_LENGTH] = {0};
	long password = 0;
	bool registered = check_id(fp, id, &password, f);
	if (registered) {
		if(f == stdin) {
    	printf("Please enter your password > ");
    }
		char pw1[MAX_PASSWORD_LENGTH] = {0};
		fscanf(f,"%s", pw1);
		while (!check_password(pw1, password)) {
			printf("Please try again > ");
			fscanf(f, "%s", pw1);
		}
		printf("successfully logged in! \n\n");
		snprintf(result, MAX_FOLDER_PATH_LENGTH, "%s%s/", BASE_LOGIN_FOLDER, id);
	} else {
		printf("No account with the associated ID was found. Would you like to register? [y/n] > ");
		char to_register;
		fscanf(f, " %c", &to_register);
		if (to_register == 'y' || to_register == 'Y') {
			printf("\n");
			result = register_new(fp, id);
		} else {
			printf("\n");
			result = login(f);
		}
	}
	fclose(fp);
	return result;
}

/* Calls Python function to authenticate email on SMTP server */
bool connect(char* email, char* password) {
	bool result = true;
	char command[256] = {0};
	snprintf(command, sizeof(command), "python3 ./send_email.py %s %s", 
    email, password);
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

/** Builds and returns a merchant's initialised data, includes :
 *  * path to merchant's folder where the menu and receipts will be stored
 *  * merchant's email and password to send receipts from
 *  * merchant's menu parsed as a linked list
 *  * initially empty list of unpaid orders
 *  * input/output files to read and write statements from
 */ 
merchant_t *login_and_init(char *input, char *output) {
  // Create merchant 
  merchant_t *merchant = calloc(1, sizeof(merchant_t));
  if (!merchant) {
    perror("Cannot make merchant");
    exit(EXIT_FAILURE);
  }
  // Assign input, output streams 
  merchant->input = input ? fopen(input, "r") : stdin;
  merchant->output = output ? fopen(output, "w") : stdout;
  if (!merchant->input || !merchant->output) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  // Attempt to login, Assign folder path 
  char* folder_path = login(merchant->input);
  merchant->folder_path = folder_path;
  // Merchant's email login authentication, Assign email and password 
	// Currently only support gmail accounts allowing access to less secure apps
  char *email = calloc(MAX_EMAIL_LENGTH, sizeof(char));
  char *password = calloc(MAX_PASSWORD_LENGTH, sizeof(char));
  if (merchant->input == stdin) {
    // interactive mode
    fprintf(stdout, "To allow us to send email receipts to customers,\n");
    fprintf(stdout, "Please enter your email address > ");
    fscanf(merchant->input, "%s", email);
    fprintf(stdout, "Please enter your password > ");
    fscanf(merchant->input, "%s", password);
    while (!connect(email, password)) {
      printf("Authentication error!\n\n");
      fprintf(stdout, "Please enter your email address > ");
      fscanf(merchant->input, "%s", email);
      fprintf(stdout, "Please enter your password > ");
      fscanf(merchant->input, "%s", password);
      printf("Authentication successful!\n");
    }
  } else {
    // assume info given in testing mode correct
		fscanf(merchant->input, "%s", email);
		fscanf(merchant->input, "%s", password);
		printf("Authentication successful!\n");
	}
  merchant->email = email;
  merchant->password = password;
  // Make merchant's menu 
	menu_t* menu = MENU_NEW();
  char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
	snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", folder_path, MENU_NAME);
  parse_menu(path_to_menu, menu);
  merchant->menu = menu;
  merchant->unpaid_orders = unpaid_list_new();
  return merchant;
}
