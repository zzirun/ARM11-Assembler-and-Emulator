#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define PERMISSION_BITS (0777)

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
bool check_user_id(FILE* fp, char* user_id, long* password) {
	printf("Please enter you user ID to login > ");
	scanf("%s", user_id);
	char str[20] = {0};
	bool registered = false;
	const char delim[2] = " "; //Split at space
	while (fgets(str, 75, fp)) {
		str[strlen(str) - 1] = 0; //Removes new line
		char* token = strtok(str, delim);
		if (!token) { //Check for empty string
			break;
		}
		if (!strcmp(token, user_id)) {
			registered = true;
			token = strtok(NULL, delim);
			*password = atol(token); //Sets password to hash stored with associated userID in fp
			break;
		}
	}
	return registered;
}

DIR* register_user(FILE* fp, char* user_id, char* path_name) {
	printf("Please enter a password > ");
	char pw[50] = {0};
	scanf("%s", pw);
	char* pw_pepper = pepper(pw); //Adds pepper to pw
	fprintf(fp, "%s", user_id);
	fprintf(fp, " "); //Adds space between user ID and password
	fprintf(fp, "%ld\n", hash(pw_pepper));
	if (!mkdir(path_name, PERMISSION_BITS)) {
		printf("The user ID, %s has been successfully registered!\n", user_id);
		free(pw_pepper);
		return opendir(path_name);
	} else {
		perror("Failed to create folder");
		exit(EXIT_FAILURE);
	}
}

DIR* login(void) {
	DIR* result;
	FILE* fp = fopen("userID.txt", "r+"); //Open for reading and possibly writing
	char user_id[25] = {0};
	long password = 0;
	bool registered = check_user_id(fp, user_id, &password);
	char path_name[6 + strlen(user_id)];
	strcpy(path_name, "Users/");
	strcpy(&path_name[6], user_id); //Builds the path name Users/<user_id> where we store the receipts
	if (registered) {
		printf("Please enter your password > ");
		char pw1[50] = {0};
		scanf("%s", pw1);
		while (!check_password(pw1, password)) {
			printf("Please try again > ");
			scanf("%s", pw1);
		}
		printf("successfully logged in! \n");
		result = opendir(path_name);
	} else {
		printf("No account with the associated user ID was found. Would you like to register? [y/n] > ");
		char to_register;
		scanf(" %c", &to_register);
		if (to_register == 'y' || to_register == 'Y') {
			result = register_user(fp, user_id, path_name);
		} else {
			result = login();
		}
	}
	fclose(fp);
	return result;
}

//Included a main for testing
int main(void) {
	DIR* dir = login();
	closedir(dir);
	return EXIT_SUCCESS;
}
