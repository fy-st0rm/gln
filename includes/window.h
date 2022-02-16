#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "includes.h"

typedef struct 
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width, height;
} GLNWindow;

GLNWindow* 	gln_create_window		(const char* title, int w, int h);
void		gln_destroy_window		(GLNWindow* window);
void 		gln_clear_window		(GLNWindow* window, vec4f color);
void		gln_update_window		(GLNWindow* window);

#endif
