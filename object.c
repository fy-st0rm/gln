#include "object.h"


Object* gln_create_object(vec2f pos, vec2f size, vec4f color)
{
	Object* object = malloc(sizeof(Object));
	object->pos = pos;
	object->size = size;
	object->color = color;
	object->quad = gln_create_quad(pos.x, pos.y, size.x, size.y, color);
	return object;
}

void gln_render_object(GLNRenderer* renderer, Object* object)
{
	gln_push_quad(renderer, object->quad);
}
