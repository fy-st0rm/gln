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
	}
}

void entity_add_force(Entity* entity, char dir, float f)
{
	if (dict_exists(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT)))
	{
		PhysicsComponentStruct* phy_comp = dict_get(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT));
		if (dir == 'X')
		{
			if (phy_comp->f_dir.x != (f / fabsf(f)))
			{
				phy_comp->force.x = 0;
				phy_comp->f_dir.x = f / fabsf(f);
			}
			else
				phy_comp->force.x = f;

			if (phy_comp->force.x == 0 && phy_comp->net_force.x == 0)
				phy_comp->net_force.x = f;
		}
		else if (dir == 'Y')
			phy_comp->force.y = f;

		// NOTE: Not supported for `Z` axis
	}
}

/*
 * Physics handler for entities
 */

void entity_update_physics(Entity* entity, float dt)
{
	if (!dict_exists(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT))) return;

	// Getting physics component and checking if it's a dynamic component
	PhysicsComponentStruct* phy_comp = dict_get(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT));
	if (!phy_comp->dynamic) return;

	if (phy_comp->velocity.x != 0)
	{
		// Calculating and appliying friction force
		phy_comp->friction = phy_comp->coff_friction * phy_comp->mass * phy_comp->g;
		phy_comp->net_force.x -= phy_comp->friction * phy_comp->f_dir.x;
		phy_comp->net_force.z -= phy_comp->friction * phy_comp->f_dir.z;

		// Nulling the net force if (Net Force == Friction force)
		if (fabsf(phy_comp->net_force.x) <= fabsf(phy_comp->friction))
		{
			phy_comp->net_force.x = 0;
			phy_comp->net_force.z = 0;
		}
	}
	else
	{
		phy_comp->net_force = phy_comp->force;
	}

	// Calculating the velocity 
	phy_comp->velocity.x += (phy_comp->net_force.x / phy_comp->mass) * dt; 
	phy_comp->velocity.y += (phy_comp->force.y / phy_comp->mass) * dt; 
	phy_comp->velocity.z += (phy_comp->net_force.z / phy_comp->mass) * dt;

	// Gravity
	phy_comp->force.y += phy_comp->mass * phy_comp->g;

	// Deceleration when force is 0
	if (phy_comp->force.x == 0)
	{
		phy_comp->velocity.x = 0;
	}

	// Reseting the force
	phy_comp->force.x = 0;
}

void entity_update_collision(Entity* entity, Entity** entities, int cnt)
{
	// Checking if the entity has physics and box collider component
	if (!dict_exists(entity->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT)))return;
	if (!dict_exists(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT))) return;

	// Getting the physics component and checking if it a dymaic phyiscs component 
	PhysicsComponentStruct* phy_comp = dict_get(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT));
	if (!phy_comp->dynamic) return;

	// Getting box collider and reseting the hits
	BoxColliderComponentStruct* a = dict_get(entity->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT));
	a->left_hit = a->right_hit = a->up_hit = a->down_hit = false;

	for (int i = 0; i < cnt; i++)
	{
		if (entity->idx == i) continue;
		if (!dict_exists(entities[i]->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT))) continue;

		BoxColliderComponentStruct* b = dict_get(entities[i]->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT));

		// For Y axis
		entity->pos.y += phy_comp->velocity.y;
		a->rect.x = entity->pos.x;
		a->rect.y = entity->pos.y;
		b->rect.x = entities[i]->pos.x;
		b->rect.y = entities[i]->pos.y;

		if (box_collider_intersect(a, b))
		{
			// Ground collision
			if (phy_comp->velocity.y > 0)
			{
				a->down_hit = true;
				entity->pos.y = entities[i]->pos.y - entity->size.y;
				phy_comp->velocity.y = 0;
				phy_comp->force.y = 0;
			}

			// Celing collision
			else if (phy_comp->velocity.y < 0)
			{
				a->up_hit = true;
				entity->pos.y = entities[i]->pos.y + entities[i]->size.y;
				phy_comp->velocity.y = 0;
				phy_comp->force.y = 0;
			}
		}

		// For X axis
		entity->pos.x += phy_comp->velocity.x;

		a->rect.x = entity->pos.x;
		a->rect.y = entity->pos.y;
		b->rect.x = entities[i]->pos.x;
		b->rect.y = entities[i]->pos.y;

		if (box_collider_intersect(a, b))
		{
			// Right collision
			if (phy_comp->velocity.x > 0)
			{
				a->right_hit = true;
				entity->pos.x = entities[i]->pos.x - entity->size.x;
				phy_comp->velocity.x = 0;
				phy_comp->force.x = 0;
			}

			// Left collision
			else if (phy_comp->velocity.x < 0)
			{
				a->left_hit = true;
				entity->pos.x = entities[i]->pos.x + entities[i]->size.x;
				phy_comp->velocity.x = 0;
				phy_comp->force.x = 0;
			}
		}
	}
}
