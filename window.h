#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "includes.h"

typedef struct 
{
	GLFWwindow* window;
	int width, height;
} GLNWindow;

GLNWindow* 	gln_create_window		(const char* title, int w, int h);
void		gln_destroy_window		(GLNWindow* window);
void 		gln_clear_window		(GLNWindow* window, vec4f color);
bool		gln_window_running		(GLNWindow* window);
void		gln_update_window		(GLNWindow* window);

#endif
