#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//NOTE: These components can cause memory leaks so be carefull

#include "../gln.h"

// Components names
#define RENDER_COMPONENT       "<render_component>"
#define ANIMATION_COMPONENT    "<animation_component>"
#define BOX_COLLIDER_COMPONENT "<box_collider_component>"
#define PHYSICS_COMPONENT	   "<physics_component>"


/*
 * Entity Render Component:
 * - Deals with the rendering of the entities
 */

typedef struct
{
	GLNRenderer* renderer;
	vec4f color;
	vec4f tex_cord;
	GLNTexture texture;
} RenderComponentStruct;

RenderComponentStruct* render_component(GLNRenderer* renderer, vec4f color, vec4f tex_cord, GLNTexture texture);

/*
 * Entity Animation Component:
 * - Adds animation to the entity
 */

typedef struct
{
	Dict* database;
	char curr_frame[256];
	float idx;
	float speed;
} AnimationComponentStruct;

AnimationComponentStruct* animation_component(char* def_frame, float speed);
void animation_add_frame(AnimationComponentStruct* animation, char* frame_name, List* tex_cords);
void animation_change_frame(AnimationComponentStruct* animation, char* frame);
vec4f* animation_get_frame(AnimationComponentStruct* animation);

/*
 * Box Collider Component
 * - Adds a 2D box collider to the entity
 */

typedef struct
{
	vec4f rect;
	bool left_hit, right_hit, up_hit, down_hit;
} BoxColliderComponentStruct;

BoxColliderComponentStruct* box_collider_component(float x, float y, float w, float h);
bool box_collider_intersect(BoxColliderComponentStruct* a, BoxColliderComponentStruct* b);

/*
 * Physics Component
 * - Adds 2D physics system
 */

typedef struct
{
	float mass;
	vec3f velocity;
	vec3f force;
	vec3f net_force;
	vec3f f_dir;
	float friction;
	float coff_friction;
	float g;

	bool dynamic;
} PhysicsComponentStruct;

PhysicsComponentStruct* physics_component(float mass, float coff_friction, float g, bool dynamic);

#endif
