#include "../includes/dict.h"

Dict* dict_new()
{
	Dict* dict  = (Dict*) malloc(sizeof(Dict));
	dict->items = (Dict_item**) malloc(sizeof(Dict_item) * 2);
	dict->size  = 0;
	return dict;
}

void dict_clean(Dict* dict)
{
	free(dict->items);
	free(dict);
}

void dict_print(Dict* dict)
{
	for (int i = 0; i < dict->size; i++)
	{
		Dict_item* item = dict->items[i];
		printf("{%p : %p} ", item->key, item->value);
	}
	printf("\n");
}

void dict_insert(Dict* dict, void* key, void* value) 
{
	dict->size++;
	Dict_item** new_items = (Dict_item**) malloc(sizeof(Dict_item) * dict->size * 2);
	memcpy(new_items, dict->items, sizeof(dict->items));

	new_items[dict->size - 1] = (Dict_item*) malloc(sizeof(Dict_item));
	new_items[dict->size - 1]->key = key;
	new_items[dict->size - 1]->value = value;

	free(dict->items);
	dict->items = new_items;
}

void dict_pop(Dict* dict, void* key)
{
	if (dict->size == 0)
	{
		fprintf(stderr, "[ERROR]: Cannot pop empty dictionary.\n");
		exit(1);
	}

	Dict_item** new_items = (Dict_item**) malloc(sizeof(Dict_item) * (dict->size - 1) * 2);
	int prev_size = dict->size;

	int index = 0;
	for(int i = 0; i < prev_size; i++)
	{
		Dict_item* item = dict->items[i];
		printf("%d\n", index);
		if (key != item->key)
		{
			new_items[index] = item;
			index++;
		}
		else
		{
			dict->size--;
		}
	}

	if (prev_size == dict->size)
	{
		fprintf(stderr, "[ERROR]: Didnt found the required key.\n");
		exit(1);
	}

	free(dict->items);
	dict->items = new_items;
}

void* dict_get(Dict* dict, void* key)
{
	for(int i = 0; i <  dict->size; i++)
	{
		Dict_item* item = dict->items[i];
		if (key == item->key)
		{
			return item->value;
		}
	}
	fprintf(stderr, "[ERROR]: Couldnt find the key you were searching for.\n");
	exit(1);
	return NULL;
}

