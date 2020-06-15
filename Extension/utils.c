#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "types.h"

list_elem_t *list_elem_new(void) {
  list_elem_t *elem = calloc(1, sizeof(list_elem_t));
  if (!elem) {
    perror("Item cannot be created");
    exit(EXIT_FAILURE);
  }
	return elem;
}

list_t *list_new(void) {
	list_t *result = calloc(1, sizeof(list_t));
	result->head = result->tail = list_elem_new();
	return result;
}

bool add_menu_item(char *item_str, menu_t *menu) {
  menu_item_t *menu_item = calloc(1, sizeof(menu_item_t));
  if (!menu_item) {
    return false;
  }
  // Set id
  char *field = strtok(item_str, " ");
  menu_item->id = atoi(field);
  // Set name
  field = strtok(NULL, " ");
  char *name = calloc(strlen(field) + 1, sizeof(char));
  if (!name) {
    return false;
  }
  strcpy(name, field);
  menu_item->name = name;
  // Set price
  menu_item->price = atof(strtok(NULL, " "));
  // Add to menu
	menu->tail->next = menu_item;
	menu->tail = menu_item;
  return true;
}

void free_list_elem(list_elem_t *item) {
	free(item->name);
	free(item);
}

void free_list(list_t *list) {
	list_elem_t *curr = list->head->next;
	while (curr) {
		list_elem_t *temp = curr;
		curr = curr->next;
		free_list_elem(temp);
	}
	free(list->head);
	free(list);
}

order_t *order_item(menu_t *menu, int id) {
	order_t *order = MENU_ITEM_NEW();
	menu_item_t *curr = menu->head->next;
	while (curr && curr->id != id) {
		curr = curr->next;
	}
  if (!curr) {
    fprintf(stderr, "Menu item not available");
    exit(EXIT_FAILURE);
  }
	char *item_name = calloc(strlen(curr->name) + 1, sizeof(char));
  if (!item_name) {
    perror("Menu item cannot be orderd");
    exit(EXIT_FAILURE);
  }
	strcpy(item_name, curr->name);
  order->id = id;
	order->name = item_name;
	order->price = curr->price;
	return order;
}

void add_order(order_t *order, order_list_t *list) {
	order_t *prev = list->head;
	order_t *curr = list->head->next;
	while (curr && (curr->id < order->id)) {
		prev = curr;
		curr = curr->next;
	}
	if (!curr) { //To add new order at the end of list
		list->tail->next = order;
	  list->tail = order;
	} else if (curr->id == order->id) {
		curr->quantity += order->quantity;
		FREE_ORDER(order);
	} else {
		order->next = curr;
		prev->next = order;
	}
}

/ *mode 0 to print menu, mode 1 to print order_list */
void print_list(list_t *list, int mode) {
	list_elem_t *curr = list->head->next;
	printf("\n");
	while (curr) {
		printf("\t %-3d %-30s %8.2f ", curr->id, curr->name, curr->price);
		if (mode) {
			printf("%5d", curr->quantity);
		}
		printf("\n");
		curr = curr->next;
	}
}

receipt_t *make_receipt(order_list_t *order_list, float total) {
  receipt_t *receipt = calloc(1, sizeof(receipt_t));
  if (!receipt) {
    perror("Receipt cannot be created");
    exit(EXIT_FAILURE);
  }
	receipt->order_list = order_list;
	receipt->total_amount = total;
}

void print_receipt(receipt_t *receipt) {
  printf("\n\t Your receipt : ");
  PRINT_ORDER_LIST(receipt->order_list);
  char *final_output = "Your total amount due is      :";
	printf("\t %-34s %8.2f\n\n", final_output, receipt->total_amount);
}

/ *Lists all files in the directory specified by path, printed with indent */
void list_all_files(DIR *path) {
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
