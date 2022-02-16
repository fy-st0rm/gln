#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "includes.h"
#include "window.h"

typedef struct
{
	unsigned int id;
	int width, height;
} GLNTexture;

GLNTexture gln_load_texture(GLNWindow* window, char* file_path);

#endif

