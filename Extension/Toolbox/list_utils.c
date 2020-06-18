#include "types.h"

/* Initializes a new list item */
list_elem_t *list_elem_new(void) {
  return calloc(1, sizeof(list_elem_t));
}

/** Initializes a new list
 * returns NULL if either the list or the list's head fails to allocate */
list_t *list_new(void) {
	list_t *result = calloc(1, sizeof(list_t));
  if (result) {
    result->head = result->tail = list_elem_new();
  }
	return (!result || !result->head) ? NULL : result;
}

/* Adds elem to end of list */
void add_to_list(list_elem_t *elem, list_t *list) {
  list->tail->next = elem;
	list->tail = elem;
}

/* Frees item in list */
void free_list_elem(list_elem_t *item) {
	free(item->name);
	free(item);
}

/* Frees contents of list including list itself */
void free_list(list_t *list) {
  if (!list){return;}
	list_elem_t *curr = list->head->next;
	while (curr) {
		list_elem_t *temp = curr;
		curr = curr->next;
		free_list_elem(temp);
	}
	free(list->head);
	free(list);
}
