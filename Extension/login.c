#include "login.h"
#include <stdio.h>

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
	long hash = 1906; //Project Deadline :D
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
	snprintf(path_name, MAX_FOLDER_PATH_LENGTH, "%s%s", base_login_folder, id);
	if (!mkdir(path_name, PERMISSION_BITS)) {
		printf("Thank you for joining us as a merchant! Please drag and drop your menu.txt into Merchants/%s\n" ,id);
		printf("Press any key when you have done so to continue > ");
		getchar(); //To skip the \n from previous input
		getchar();
		char path_to_menu[MAX_MENU_PATH_LENGTH] = {0};
		snprintf(path_to_menu, MAX_MENU_PATH_LENGTH, "%s/%s", path_name, "menu.txt");
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
			result = register_new(fp, id);
		} else {
			result = login(f);
		}
	}
	fclose(fp);
	return result;
}

void login_d(FILE *f){
    char pw1[MAX_PASSWORD_LENGTH] = {0};
    fscanf(f,"%s", pw1);
    printf("%s \n", pw1);
}

