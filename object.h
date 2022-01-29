#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "renderer.h"

typedef struct
{
	vec2f pos;
	vec2f size;
	vec4f color;
	Quad* quad;
} Object;

Object*   gln_create_object    (vec2f pos, vec2f size, vec4f color);
void      gln_render_object    (GLNRenderer* renderer, Object* object);

#endif
