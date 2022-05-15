#include "../../includes/glnx/component.h"

/*
 * Entity Render Component
 */

RenderComponentStruct* render_component(GLNRenderer* renderer, vec4f color, vec4f tex_cord, GLNTexture texture)
{
	RenderComponentStruct* component = malloc(sizeof(RenderComponentStruct));
	component->renderer = renderer;
	component->color = color;
	component->tex_cord = tex_cord;
	component->texture = texture;
	return component;
}


/*
 * Animation Component
 */

AnimationComponentStruct* animation_component(char* def_frame, float speed)
{
	AnimationComponentStruct* component = malloc(sizeof(AnimationComponentStruct));
	component->database = dict_new();
	strcpy(component->curr_frame, def_frame);
	component->idx = 0.0f;
	component->speed = speed;
	return component;
}

void animation_add_frame(AnimationComponentStruct* animation, char* frame_name, List* tex_cords)
{
	dict_insert(animation->database, (void*) frame_name, (void*) tex_cords, sizeof(char) * strlen(frame_name), tex_cords->size * tex_cords->size); 
}

void animation_change_frame(AnimationComponentStruct* animation, char* frame)
{
	if (strcmp(animation->curr_frame, frame)) 
	{
		strcpy(animation->curr_frame, frame);
		animation->idx = 0.0f;
	}
}

vec4f* animation_get_frame(AnimationComponentStruct* animation)
{
	List* list = dict_get(animation->database, (void*) animation->curr_frame, sizeof(char) * strlen(animation->curr_frame));
	vec4f* cords = list_get(list, (int)round(animation->idx));

	animation->idx += 1.0f / animation->speed;
	if ((int)round(animation->idx) >= list->len)
		animation->idx = 0.0f;

	return cords;
}

void animation_set_state(AnimationComponentStruct* animation, char* frame, float speed, bool condition)
{
	if (condition)
	{
		animation_change_frame(animation, frame);
		animation->speed = speed;
	}
}

/*
 * Box Collider Component
 */

BoxColliderComponentStruct* box_collider_component(float x, float y, float w, float h)
{
	vec4f rect = { x, y, w, h };
	BoxColliderComponentStruct* component = malloc(sizeof(BoxColliderComponentStruct));
	component->rect = rect;
	component->left_hit = component->right_hit = component->up_hit = component->down_hit = false;
	return component;
}

bool box_collider_intersect(BoxColliderComponentStruct* a, BoxColliderComponentStruct* b)
{
	//TODO: Bug in collision detection
	vec4f rect_a = a->rect;
	vec4f rect_b = b->rect;

	if(rect_a.x < rect_b.x + rect_b.z && rect_a.x + rect_a.z > rect_b.x && 
			rect_a.y < rect_b.y + rect_b.w && rect_a.y + rect_a.w > rect_b.y)
		return true;
	return false;

	/*
	if ((rect_a.x < rect_b.x && rect_b.x < rect_a.x + rect_a.z) || (rect_b.x < rect_a.x && rect_a.x < rect_b.x + rect_b.z))
		if ((rect_a.y < rect_b.y && rect_b.y < rect_a.y + rect_a.w) || (rect_b.y < rect_a.y && rect_a.y < rect_b.y + rect_b.w))
		   return true;
	*/

	/*
	if ((rect_b.x < rect_a.x && rect_a.x < rect_b.x + rect_b.z) && (rect_b.y < rect_a.y && rect_a.y < rect_b.y + rect_b.w)) return true;
	if ((rect_b.x < rect_a.x + rect_a.z && rect_a.x + rect_a.z < rect_b.x + rect_b.z) && (rect_b.y < rect_a.y && rect_a.y < rect_b.y + rect_b.w)) return true;
	if ((rect_b.x < rect_a.x && rect_a.x < rect_b.x + rect_b.z) && (rect_b.y < rect_a.y + rect_a.w && rect_a.y + rect_a.w < rect_b.y + rect_b.w)) return true;
	if ((rect_b.x < rect_a.x + rect_a.z && rect_a.x + rect_a.z < rect_b.x + rect_b.z) && (rect_b.y < rect_a.y + rect_a.w && rect_a.y + rect_a.w < rect_b.y + rect_b.w)) return true;
	*/
	return false;
}

/*
 * Physcs Component
 */

PhysicsComponentStruct* physics_component(float mass, float coff_friction, float g, bool dynamic, bool apply_gravity)
{
	PhysicsComponentStruct* component = malloc(sizeof(PhysicsComponentStruct));
	vec3f temp = { 0.0f, 0.0f, 0.0f };
	component->velocity  = component->force = component->net_force = temp;
	component->f_dir = temp;
	component->mass = mass;
	component->coff_friction = coff_friction;
	component->g = g;
	component->dynamic = dynamic;
	component->apply_gravity = apply_gravity;
	return component;
}
