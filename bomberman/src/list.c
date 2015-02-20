#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <list.h>

struct list_element {
        void* value;
        struct list_element* next;
};

struct list {
        struct list_element* first;
        struct list_element* last;
        int size;
};

struct list* list_init() {
        struct list* list = malloc(sizeof(struct list));
        list->first = NULL;
        list->last = NULL;
        list->size = 0;
        return list;
}

void list_add(struct list* list, void* value) {
assert(list);
        struct list_element* elt = malloc(sizeof(struct list_element));

        if (list->size == 0) {
                    list->first = elt;
        }
        elt->value = value;
        elt->next = NULL;
        if(list->last!=NULL){
        list->last->next = elt;
        }

        list->last = elt;



        list->size++;
}

void list_destroy(struct list* list) {
	assert(list);
        free(list);
}

// supprime le struct list_element* qui a la valeur "value"
// on part du principe que les value sont uniques dans la liste
void list_remove(struct list* list, void* value) {
	assert(list);
	assert(value);

	struct list_element * current = list->first;

if (current->value==value){
	if(list->size==1){
		list->first=NULL;
		list->last=NULL;

	}else {
		list->first=current->next;
	}
	list->size--;
	return;
}

	while(current->value != NULL){
		if(current->next->value==value){
			current->next=(current->next)->next;
			list->size--;
			break;
		}
		current=list_get_next(current);
	}
}

void list_apply(struct list* list, void (*fonction)(void*)){
	assert(list);
        struct list_element * current = list->first;
        while(current!= NULL){
                fonction(current);
                current=current->next;
        }


}

struct list_element * list_get_first(struct list * list){
	assert(list);
return list->first;
}

struct list_element * list_get_next(struct list_element * element){
	assert(element);
return element->next;
}

void* list_get_value(struct list_element * element){
assert(element);
return element->value;
}

