#include "../includes/cutil.h"

/*
 * Dynamic list
 */

List* list_new()
{
	List* list = malloc(sizeof(List));
	list->size = 5;
	list->len = 0;
	list->items = malloc((list->size * list->size) * sizeof(void*));
	return list;
}

void list_clean(List* list)
{
	free(list->items);
	free(list);
}

void list_append(List* list, void* item, size_t size)
{
	if (list->len == list->size)
	{
		list->size += 5;
		void** new_items = malloc((list->size * list->size) * sizeof(void*));
		memcpy(new_items, list->items, sizeof(list->items) * list->len);
		free(list->items);
		list->items = new_items;
	}
	list->items[list->len] = calloc(1, size);
	memcpy(list->items[list->len], item, size);
	list->len++;
}

void* list_get(List* list, int index)
{
	if (index >= list->len)
	{
		fprintf(stderr, "[ERROR]: List index out of range.\n");
		exit(1);
	}
	return list->items[index];
}

void list_print(List* list)
{
	for (int i = 0; i < list->len; i++)
		printf("%p ", list->items[i]);
	printf("\n");
}

void list_pop(List* list, int index)
{
	if (index >= list->len)
	{
		fprintf(stderr, "[ERROR]: List index out of range.\n");
		exit(1);
	}
	for (int i = index; i < list->len; i++)
	{
		list->items[i] = list->items[i+1];
	}
	list->len--;
}


/*
 * Dynamic dictionary
 */

Dict* dict_new()
{
	Dict* dict  = (Dict*) malloc(sizeof(Dict));
	dict->size  = 5;
	dict->len   = 0;
	dict->items = (Dict_item**) malloc(sizeof(Dict_item) * (dict->size * dict->size) * 2);
	return dict;
}

void dict_clean(Dict* dict)
{
	free(dict->items);
	free(dict);
}

void dict_print(Dict* dict)
{
	for (int i = 0; i < dict->len; i++)
	{
		Dict_item* item = dict->items[i];
		printf("{%p : %p} ", item->key, item->value);
	}
	printf("\n");
}

void dict_insert(Dict* dict, void* key, void* value, size_t k_sz, size_t v_sz) 
{
	if (dict->len >= dict->size)
	{
		// Increasing the size of the dictionary and swaping the pointers
		dict->size += 5;
		void** new_items = malloc(2 * (dict->size * dict->size) * sizeof(Dict_item));
		memcpy(new_items, dict->items, sizeof(dict->items) * dict->len);
		free(dict->items);
		dict->items = new_items;
	}

	dict->items[dict->len] = (Dict_item*) malloc(sizeof(Dict_item));
	dict->items[dict->len]->key   = calloc(1, k_sz);
	memcpy(dict->items[dict->len]->key, key, k_sz);
	dict->items[dict->len]->value = calloc(1, v_sz);
	memcpy(dict->items[dict->len]->value, value, v_sz);
	dict->len++;
}

void dict_pop(Dict* dict, void* key, size_t k_sz)
{
	if (dict->len == 0)
	{
		fprintf(stderr, "[ERROR]: Cannot pop empty dictionary.\n");
		exit(1);
	}

	int prev_size = dict->len;

	int index = 0;
	for(int i = 0; i < prev_size; i++)
	{
		Dict_item* item = dict->items[i];
		if (memcmp(key, item->key, k_sz))
		{
			dict->items[index] = item;
			index++;
		}
		else
		{
			dict->len--;
		}
	}

	if (prev_size == dict->len)
	{
		fprintf(stderr, "[ERROR]: Didnt found the required key.\n");
		exit(1);
	}
}

void* dict_get(Dict* dict, void* key, size_t k_sz)
{
	for(int i = 0; i <  dict->len; i++)
	{
		Dict_item* item = dict->items[i];
		if (!memcmp(key, item->key, k_sz))
		{
			return item->value;
		}
	}
	fprintf(stderr, "[ERROR]: Couldnt find the key you were searching for.\n");
	exit(1);
	return NULL;
}

Dict_item* dict_get_item_idx(Dict* dict, int idx)
{
	if (idx >= dict->len)
	{
		fprintf(stderr, "[ERROR]: Index out of bounds.\n");
		exit(1);
	}

	Dict_item* item = dict->items[idx];
	return item;
}

bool dict_exists(Dict* dict, void* key, size_t k_sz)
{
	for (int i = 0; i < dict->len; i++)
	{
		Dict_item* item = dict->items[i];
		if (!memcmp(key, item->key, k_sz))
		{
			return true;
		}
	}
	return false;
}
