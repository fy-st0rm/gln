#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "../gln.h"
#include "component.h"

/*
 * Entity struct
 * - Acts as an object for the game
 */

typedef struct
{
	vec3f pos;
	vec2f size;
	float ang;
	int idx;	// Index of it in entities list
	Dict* components;
} Entity;


/*
 * Entity manager
 */

#define MAX_ENTITY 10000
typedef struct
{
	Entity* entities[MAX_ENTITY];
	int     entity_cnt;

	// These indices points to the entity having the following components in the entities list
	int		ren_comp_indices[MAX_ENTITY];
	int     box_comp_indices[MAX_ENTITY];
	int     physics_com_indices[MAX_ENTITY];
} EntityManager;

EntityManager* entity_manager_new();
void		   entity_manager_destroy(EntityManager* e_manager);
void           entity_update(EntityManager* e_manager, GLNRenderer* renderer, float dt);

Entity* entity_new(EntityManager* e_manager, vec3f pos, vec2f size);
void	entity_destroy(Entity* entity);
void    entity_add_component(Entity* entity, char* name, void* component, size_t comp_size);
void*   entity_get_component(Entity* entity, char* name);
vec3f   entity_get_gl_pos(Entity* entity, int screen_width, int screen_height);

// Component based functions
void	entity_render(Entity* entity);
void    entity_add_force(Entity* entity, char dir, float f);
void    entity_update_physics(Entity* entity, float dt);
void    entity_update_collision(Entity* entity, Entity** entities, int cnt);

#endif
