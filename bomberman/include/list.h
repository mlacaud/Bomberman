#ifndef LIST_H_
#define LIST_H_

struct list_element;
struct list;

struct list* list_init();

//Add a element to the list
void list_add(struct list* list, void* value);

//Delete a list
void list_destroy(struct list* list);

// Delete the struct list_element* which has the value "value".
void list_remove(struct list* list, void* value);

//Apply to all the elements of the list a function
void list_apply(struct list* list, void (*fonction)(void *));

// Return the first element of the list
struct list_element * list_get_first(struct list * list);

// Return the next element of the list
struct list_element * list_get_next(struct list_element * element);

// Return the value of one element
void* list_get_value(struct list_element * element);

#endif /* LIST_H_ */
