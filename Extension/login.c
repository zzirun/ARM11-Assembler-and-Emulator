#include "merchant.h"

#define MAX_ID_LENGTH (20)
#define PERMISSION_BITS (0777)
#define MAX_PASSWORD_LENGTH (50)
#define MAX_FOLDER_PATH_LENGTH (30) //strlen("Merchants/") + strlen(user_id)
#define MAX_MENU_PATH_LENGTH (35) //strlen(folder_path) + strlen("menu.txt")


/* Appends a random ASCII character between 33 ('!') and 122 ('z'),
this process is known as "peppering", it is done to improve security */
char* pepper(char* str) {
	srand(time(NULL)); //Seed with time
	int size = strlen(str) + 1; //Account for sentinel character
	char* str_copy = calloc(size + 1, sizeof(char)); //Make room for pepper, set all to 0
	strcpy(str_copy, str);
	str_copy[size - 1] = rand() % 89 + 33; //Set pepper
	return str_copy;
}

//djb2 hash with pepper
long hash(char* to_hash) {
	/* Magic constant which produces better avalanching (changing the input slightly causes massive change in output) */
	long hash = 5381;
  while (*to_hash) {
		hash = ((hash << 5) + hash) + *to_hash;
		to_hash++;
	}
  return hash;
}

/* Returns true if the input (pw) after
hashing matches the hash (hash_pw) */
bool check_password(char* pw, long hash_pw) {
	char pw_copy[strlen(pw) + 2];
	pw_copy[sizeof(pw_copy) - 1] = 0;
	strcpy(pw_copy, pw);
	/* Brute force attempt to check the
	pepper appended during registration */
	for (int i = 33; i < 123; i++) {
		pw_copy[sizeof(pw_copy) - 2] = i;
		if (hash(pw_copy) == hash_pw) {
			return true;
		}
	}
	return false;
}

/* If user_id is in fp, we update password with
the hash value stored in fp and return true */
bool check_id(FILE* fp, char* id, long* password, FILE *f) {
  if(f == stdin) {
		printf("Please enter you user ID to login > ");
  }
	fscanf(f,"%s", id);
	char str[MAX_ID_LENGTH] = {0};
	bool registered = false;
	const char delim[2] = " "; //Split at space
	while (fgets(str, MAX_ID_LENGTH + MAX_PASSWORD_LENGTH, fp)) {
		str[strlen(str) - 1] = 0; //Removes new line
		char* token = strtok(str, delim);
		if (!token) { //Check for empty string
			break;
		}
		if (!strcmp(token, id)) {
			registered = true;
			token = strtok(NULL, delim);
			*password = atol(token); //Sets password to hash stored with associated userID in fp
			break;
		}
	}
	return registered;
}

char* register_new(FILE* fp, char* id) {
	printf("Please enter a password > ");
	char pw[MAX_PASSWORD_LENGTH] = {0};
	scanf("%s", pw);
	char* path_name = calloc(MAX_FOLDER_PATH_LENGTH, sizeof(char));
	snprintf(path_name, MAX_FOLDER_PATH_LENGTH, "%s%s/", base_login_folder, id);
	if (!mkdir(path_name, PERMISSION_BITS)) {
		printf("Thank you for joining us as a merchant! Please drag and drop your menu.txt into Merchants/%s\n" ,id);
		printf("Press any key when you have done so to continue > ");
		getchar(); //To skip the \n from previous input
		getchar();
		char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
		snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", path_name, "menu.txt");
		while (!fopen(path_to_menu, "r")) {
			printf("menu.txt not found! Please make sure it is at the right location and named correctly.\n");
			printf("Press any key when you have done so to continue > ");
			getchar();
		}
		char* pw_pepper = pepper(pw); //Adds pepper to pw
		fprintf(fp, "%s", id);
		fprintf(fp, " "); //Adds space between user ID and password
		fprintf(fp, "%ld\n", hash(pw_pepper));
		free(pw_pepper);
		return path_name;
	} else {
		perror("Failed to create folder");
		exit(EXIT_FAILURE);
	}
}

// For interactive mode ensure f = stdin
// For testing mode f = test file
char* login(FILE *f) {
	char* result = calloc(MAX_FOLDER_PATH_LENGTH, sizeof(char));
	FILE* fp = fopen(id_data, "r+"); //Open for reading and possibly writing
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
		snprintf(result, MAX_FOLDER_PATH_LENGTH, "%s%s/", base_login_folder, id);
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

merchant_t *login_and_init(char *input, char *output) {
  merchant_t *merchant = calloc(1, sizeof(merchant_t));
  if (!merchant) {
    perror("Cannot make merchant");
    exit(EXIT_FAILURE);
  }
  /* Assign input, output streams */
  merchant->input = input ? fopen(input, "r") : stdin;
  merchant->output = output ? fopen(output, "w") : stdout;
  if (!merchant->input || !merchant->output) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  /* Attempt to login, Assign folder path */
  char* folder_path = login(merchant->input);
  merchant->folder_path = folder_path;
  /* Merchant's email login authentication, Assign email and password */
	//Currently only support gmail accounts that allow access to less secure apps
  char *email = calloc(MAX_EMAIL_LENGTH, sizeof(char));
  char *password = calloc(MAX_PASSWORD_LENGTH, sizeof(char));
  if (merchant->input == stdin) {
    // Interactive mode
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
    // Assume info given in testing mode correct
		fscanf(merchant->input, "%s", email);
		fscanf(merchant->input, "%s", password);
		printf("Authentication successful!\n");
	}
  merchant->email = email;
  merchant->password = password;
  /* Make merchant's menu */
	menu_t* menu = MENU_NEW();
  char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
	snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s%s", folder_path, menu_name);
  parse_menu(path_to_menu, menu);
  merchant->menu = menu;
  merchant->unpaid_orders = unpaid_list_new();
  return merchant;
}
