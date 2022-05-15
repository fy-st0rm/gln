#include "../../includes/glnx/entity.h"

/*
 * Entity Manager Functions
 */

EntityManager* entity_manager_new()
{
	EntityManager* e_manager = malloc(sizeof(EntityManager));
	e_manager->entity_cnt = 0;
	return e_manager;
}
void entity_manager_destroy(EntityManager* e_manager)
{
	for (int i = 0; i < e_manager->entity_cnt; i++)
		entity_destroy(e_manager->entities[i]);
	free(e_manager);
}

void entity_update(EntityManager* e_manager, GLNRenderer* renderer, float dt)
{
	// TODO: [ ] Optimize this code cuz it will be slow for large number of entities
	// Rendering
	gln_render_begin(renderer);
	for (int i = 0; i < e_manager->entity_cnt; i++)
	{
		// Updating physics for all entities
		entity_update_physics(e_manager->entities[i], dt);

		// Rendering entities
		entity_render(e_manager->entities[i]);
	}
	gln_render_end(renderer);

	// Collision updating
	for (int i = 0; i < e_manager->entity_cnt; i++)
	{
		entity_update_collision(e_manager->entities[i], e_manager->entities, e_manager->entity_cnt);
	}
}

/*
 * Entity functions
 */

Entity* entity_new(EntityManager* e_manager, vec3f pos, vec2f size)
{
	Entity* entity = malloc(sizeof(Entity));
	entity->pos = pos;
	entity->size = size;
	entity->ang = 0.0f;
	entity->components = dict_new();

	entity->idx = e_manager->entity_cnt;
	e_manager->entities[e_manager->entity_cnt++] = entity;
	return entity;
}

void entity_destroy(Entity* entity)
{
	dict_clean(entity->components);
	free(entity);
}

void entity_add_component(Entity* entity, char* name, void* component, size_t comp_size)
{
	dict_insert(entity->components, (void*)name, component, sizeof(char) * strlen(name), comp_size); 
}

void* entity_get_component(Entity* entity, char* name)
{
	return dict_get(entity->components, (void*)name, sizeof(char) * strlen(name));
}

vec3f entity_get_gl_pos(Entity* entity, int screen_width, int screen_height)
{
	vec3f pos = {
		entity->pos.x / (screen_width / 2),
	   -entity->pos.y / (screen_height / 2),
		0.0f
	};
	return pos;
}

// Component based entity functions
void entity_render(Entity* entity)
{
	if (dict_exists(entity->components, RENDER_COMPONENT, sizeof(char) * strlen(RENDER_COMPONENT)))
	{
		// Negative of half of size as an X-axis to rotate the quad from the middle of it
		vec3f pos = { -(entity->size.x / 2), 0.0f, 0.0f };
	 
		RenderComponentStruct* ren_comp = dict_get(entity->components, RENDER_COMPONENT, sizeof(char) * strlen(RENDER_COMPONENT));
		Quad* quad = gln_create_quad(ren_comp->renderer, pos, entity->size, ren_comp->color, ren_comp->tex_cord, ren_comp->texture.id);

		// Multiplying the quad with rotation matrix 
		mat4f yrot = {0};
		rotation_y(&yrot, entity->ang);
		mat4f_quad_mul(yrot, quad);

		// Shifting the quad to its original position
		for (int i = 0; i < 4; i++)
		{
			quad->v[i].pos.x += entity->pos.x + entity->size.x / 2;  // Increasing by half of size to balance the previous minus
			quad->v[i].pos.y += entity->pos.y;
		}

		gln_push_quad(ren_comp->renderer, quad);
		gln_destroy_quad(quad);

		/*
		 * Enable this to see hitbox
		if (dict_exists(entity->components, (void*) BOX_COLLIDER_COMPONENT, strlen(BOX_COLLIDER_COMPONENT) * sizeof(char)))
		{
			BoxColliderComponentStruct* box = entity_get_component(entity, BOX_COLLIDER_COMPONENT);
			vec3f pos = {box->rect.x, box->rect.y, 0.0f};
			vec2f size = {box->rect.z, box->rect.w};
			vec4f color = {1.0f, 0.5f, 0.0f, 0.5f };
			quad = gln_create_quad(ren_comp->renderer, pos, size, color, ren_comp->tex_cord, 0);
			gln_push_quad(ren_comp->renderer, quad);
			gln_destroy_quad(quad);
		}
		*/
	}
}

