#include "../includes/dict.h"

Dict* dict_new(int size)
{
	Dict* dict  = (Dict*) malloc(sizeof(Dict));
	dict->items = (Dict_item**) malloc(sizeof(Dict_item) * size * 2);
	dict->size  = size;
	dict->len   = 0;
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

void dict_insert(Dict* dict, void* key, void* value) 
{
	if (dict->len >= dict->size)
	{
		fprintf(stderr, "[ERROR]: Dictionary is full cannot add more element\n");
		exit(1);
	}
	dict->items[dict->len] = (Dict_item*) malloc(sizeof(Dict_item));
	dict->items[dict->len]->key = key;
	dict->items[dict->len]->value = value;
	dict->len++;
}

void dict_pop(Dict* dict, void* key)
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
		printf("%d\n", index);
		if (key != item->key)
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

