#include "../includes/object.h"


Object* gln_create_object(GLNRenderer* renderer, vec2f pos, vec2f size, vec4f color, int id)
{
	Object* object = malloc(sizeof(Object));
	object->pos = pos;
	object->size = size;
	object->color = color;
	object->quad = gln_create_quad(renderer, pos.x, pos.y, size.x, size.y, color, id);
	return object;
}

void gln_render_object(GLNRenderer* renderer, Object* object)
{
	gln_push_quad(renderer, object->quad);
}
