#ifndef _DICT_H_
#define _DICT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


Dict* dict_new(int size);
void  dict_clean(Dict* dict);
void  dict_print(Dict* dict);
void  dict_insert(Dict* dict, void* key, void* value, size_t v_sz);
void  dict_pop(Dict* dict, void* key);
void* dict_get(Dict* dict, void* key);

#endif
