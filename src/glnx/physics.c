#include "../../includes/glnx/entity.h"

/*
 * Physics handler for entities
 */

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

			if (phy_comp->net_force.x == 0 && phy_comp->force.x == 0)
				phy_comp->net_force.x = f;
		}
		else if (dir == 'Y')
		{
			if (phy_comp->apply_gravity)
			{
				phy_comp->force.y = f;
				phy_comp->net_force.y = f;
			}
			else
			{
				if (phy_comp->f_dir.y != (f / fabsf(f)))
				{
					phy_comp->force.y = 0;
					phy_comp->f_dir.y = f / fabsf(f);
				}
				else
					phy_comp->force.y = f;

				if (phy_comp->net_force.y == 0 && phy_comp->force.y == 0)
					phy_comp->net_force.y = f;
			}
		}

		// NOTE: Not supported for `Z` axis
	}
}

void entity_update_physics(Entity* entity, float dt)
{
	if (!dict_exists(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT))) return;

	// Getting physics component and checking if it's a dynamic component
	PhysicsComponentStruct* phy_comp = dict_get(entity->components, PHYSICS_COMPONENT, sizeof(char) * strlen(PHYSICS_COMPONENT));
	if (!phy_comp->dynamic) return;

	if (phy_comp->velocity.x != 0 || (!phy_comp->apply_gravity && phy_comp->velocity.y != 0))
	{
		// Calculating and appliying friction force
		phy_comp->friction = phy_comp->coff_friction * phy_comp->mass * phy_comp->g;
		phy_comp->net_force.x -= phy_comp->friction * phy_comp->f_dir.x;

		// Nulling the net force if (Net Force == Friction force)
		if (fabsf(phy_comp->net_force.x) <= fabsf(phy_comp->friction))
			phy_comp->net_force.x = 0;

		// For Y force
		if (!phy_comp->apply_gravity)
		{
			phy_comp->net_force.y -= phy_comp->friction * phy_comp->f_dir.y;
			if (fabsf(phy_comp->net_force.y) <= fabsf(phy_comp->friction))
				phy_comp->net_force.y = 0;
		}
	}
	else
	{
		phy_comp->net_force.x = phy_comp->force.x;
		if (!phy_comp->apply_gravity)
			phy_comp->net_force.y = phy_comp->force.y;
	}

	// Gravity
	if (phy_comp->apply_gravity)
		phy_comp->net_force.y += phy_comp->mass * phy_comp->g;

	// Calculating the velocity 
	phy_comp->velocity.x += (phy_comp->net_force.x / phy_comp->mass) * dt; 
	phy_comp->velocity.y += (phy_comp->net_force.y / phy_comp->mass) * dt; 

	// Deceleration when force is 0
	if (phy_comp->force.x == 0)
		phy_comp->velocity.x = 0;
	if (!phy_comp->apply_gravity && phy_comp->force.y == 0)
		phy_comp->velocity.y = 0;

	// Reseting the force
	phy_comp->force.x = 0;
	phy_comp->force.y = 0;

	// Movement
	entity->pos.x += phy_comp->velocity.x;
	entity->pos.y += phy_comp->velocity.y;

	if (!dict_exists(entity->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT)))return;
	BoxColliderComponentStruct* a = dict_get(entity->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT));
	a->rect.x += phy_comp->velocity.x;
	a->rect.y += phy_comp->velocity.y;

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

		// Checking for collision
		if (box_collider_intersect(a, b))
		{
			float l, r, u, d;
			l = (b->rect.x + b->rect.z) - a->rect.x;
			r = (a->rect.x + a->rect.z) - b->rect.x;
			u = (b->rect.y + b->rect.w) - a->rect.y;
			d = (a->rect.y + a->rect.w) - b->rect.y;

			// Left collision
			if (l < r && l < u && l < d)
			{
				a->left_hit = true;

				entity->pos.x = b->rect.x + b->rect.z;
				a->rect.x = b->rect.x + b->rect.z;

				phy_comp->force.x = 0;
				phy_comp->velocity.x = 0;
			}
			// Right collision
			else if (r < l && r < u && r < d)
			{
				a->right_hit = true;

				entity->pos.x = b->rect.x - a->rect.z;
				a->rect.x = b->rect.x - a->rect.z;

				phy_comp->force.x = 0;
				phy_comp->velocity.x = 0;
			}
			// Up collision
			else if (u < l && u < r && u < d)
			{
				a->up_hit = true;

				entity->pos.y = b->rect.y + b->rect.w;
				a->rect.y = b->rect.y + b->rect.w;

				phy_comp->force.y = 0;
				phy_comp->net_force.y = 0;
				phy_comp->velocity.y = 0;
			}
			// Down collision
			else if (d < r && d < u && d < l)
			{
				a->down_hit = true;

				entity->pos.y = b->rect.y - a->rect.w;
				a->rect.y = b->rect.y - a->rect.w;

				phy_comp->force.y = 0;
				phy_comp->net_force.y = 0;
				phy_comp->velocity.y = 0;
			}
		}
	}
}

