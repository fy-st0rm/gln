#include "../includes/util.h"
#include "../includes/includes.h"

char* load_file(char* file)
{
	FILE* fd;
	fd = fopen(file, "r");
	if (fd == NULL)
	{
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}

	int sz = 1;
	char ch;
	do {
		ch = fgetc(fd);
		sz++;
	} while (ch != EOF);  
	fclose(fd);

	fd = fopen(file, "r");
	if (fd == NULL)
	{
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	char* buff = calloc(sz, sizeof(char));

	int i = 0;
	while ((ch = fgetc(fd)) != EOF)
	{
		buff[i++] = ch;
	}
	buff[i] = '\0';

	fclose(fd);
	return buff;
}

void strrev(char* str)
{
	char* temp = malloc(sizeof(str));
	strcpy(temp, str);
	for (int i = strlen(temp); i > 0; i--)
	{
		str[i - 1] = temp[strlen(temp) - i];
	}
	free(temp);
}
