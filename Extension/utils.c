#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "types.h"

list_elem_t* list_elem_new(void) {
	return calloc(1, sizeof(list_elem_t));
}

list_t* list_new(void) {
	list_t* result = calloc(1, sizeof(list_t));
	result->head = result->tail = list_elem_new();
	return result;
}

void add_to_list(list_elem_t* item, list_t* list) {
	list->tail->next = item;
	list->tail = item;
}

void free_list_elem(list_elem_t* item) {
	free(item->name);
	free(item);
}

void free_list(list_t* list) {
	list_elem_t* curr = list->head->next;
	while (curr) {
		list_elem_t* temp = curr;
		curr = curr->next;
		free_list_elem(temp);
	}
	free(list->head);
	free(list);
}

list_elem_t* clone_item(list_t* menu, int id) {
	list_elem_t* result = list_elem_new();
	list_elem_t* curr = menu->head->next;
	result->id = id;
	while (curr->id != id) {
		curr = curr->next;
	}
	char* item_name = calloc(70, sizeof(char));
	strcpy(item_name, curr->name);
	result->name = item_name;
	result->price = curr->price;
	return result;
}

void add_order(list_elem_t* item, list_t* list) {
	list_elem_t* prev = list->head;
	list_elem_t* curr = list->head->next;
	while (curr && (curr->id < item->id)) {
		prev = curr;
		curr = curr->next;
	}
	if (curr == 0) { //To add new item at the end of list
		add_to_list(item, list);
	} else if (curr->id == item->id) {
		curr->quantity += item->quantity;
		free_list_elem(item);
	} else {
		item->next = curr;
		prev->next = item;
	}
}

/* mode 0 to print menu, mode 1 to print receipt*/
void print_list(list_t* list, int mode) {
	float total_amount = 0;
	list_elem_t* curr = list->head->next;
	printf("\n");
	while (curr) {
		printf("\t %-3d %-30s ", curr->id, curr->name);
		if (mode) {
			printf("%8.2f %5d", curr->price * curr->quantity, curr->quantity);
			total_amount += curr->price * curr->quantity;
		} else {
			printf("%8.2f", curr->price);
		}
		printf("\n");
		curr = curr->next;
	}
	if (mode) {
		char* final_output = "Your total amount due is      :";
		printf("\t %-34s %8.2f\n\n", final_output, total_amount);
	}
}

/* Lists all files in the directory specified by path, printed with indent */
void list_all_files(DIR* path) {
	struct dirent *dir;
	if (!path) {
		perror("Invalid path");
		exit(EXIT_FAILURE);
	}
	while ((dir = readdir(path))) {
		if (strcmp(dir->d_name, "..") && strcmp(dir->d_name, ".")) {
			printf("  %s\n", dir->d_name);
		}
	}
}
