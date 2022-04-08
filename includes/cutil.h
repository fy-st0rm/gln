#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Dynamic List (Array)
 */

typedef struct 
{
	void** items;
	int len;
	int size;
} List;

List* list_new();
void  list_clean(List* list);
void  list_append(List* list, void* item, size_t size);
void* list_get(List* list, int index);
void  list_print(List* list);
void  list_pop(List* list, int index);


/*
 * Dynamic dictionary
 */

typedef struct
{
	void* key;
	void* value;
} Dict_item;

typedef struct 
{
	Dict_item** items;
	int size;
	int len;
} Dict;

Dict* dict_new();
void  dict_clean(Dict* dict);
void  dict_print(Dict* dict);
void  dict_insert(Dict* dict, void* key, void* value, size_t k_sz, size_t v_sz);
void  dict_pop(Dict* dict, void* key, size_t k_sz);
void* dict_get(Dict* dict, void* key, size_t k_sz);
Dict_item* dict_get_item_idx(Dict* dict, int idx);

#endif
