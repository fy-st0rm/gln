#include "glnx.h"

#define MASS 1
#define GRAVITY 5
#define FRICTION 0.5

// Movement constant
#define LEFT -34
#define RIGHT 34
#define JUMP -54

int main(int argc, char** argv)
{
	int fps = 60;
	GLNXCoreApp* app = glnx_core_app_new("Test", 800, 600, 800, 600, 1000, fps);
	vec4f bg = { 0.5f, 0.5f, 0.5f, 1.0f };

	// Creating a entity
	vec3f pos = { 100.0f, 100.0f, 0.0f };
	vec2f size = { 50.0f, 50.0f };
	vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vec4f cord = { 0.0f, 0.0f , 1.0f, 1.0f };
	GLNTexture tex = { 0.0f, 0.0f, 0.0f };
	Entity* box = entity_new(app->e_manager, pos, size);
	entity_add_component(box, RENDER_COMPONENT, render_component(app->renderer, color, cord, tex), sizeof(RenderComponentStruct));
	entity_add_component(box, BOX_COLLIDER_COMPONENT, box_collider_component(pos.x, pos.y, size.x, size.y), sizeof(BoxColliderComponentStruct));
	entity_add_component(box, PHYSICS_COMPONENT, physics_component(MASS, FRICTION, GRAVITY, true, true), sizeof(PhysicsComponentStruct));

	vec3f gpos = { 0.0f, 300.0f, 0.0f };
	vec2f gsize = { 1000.0f, 50.0f };
	vec4f gcol = { 0.0f, 0.5f, 0.1f, 1.0f };
	Entity* ground = entity_new(app->e_manager, gpos, gsize);
	entity_add_component(ground, RENDER_COMPONENT, render_component(app->renderer, gcol, cord, tex), sizeof(RenderComponentStruct));
	entity_add_component(ground, BOX_COLLIDER_COMPONENT, box_collider_component(gpos.x, gpos.y, gsize.x, gsize.y), sizeof(BoxColliderComponentStruct));

	vec3f gpos1 = { -1000.0f, 250.0f, 0.0f };
	vec4f gcol1 = { 0.5f, 0.5f, 0.1f, 1.0f };
	Entity* ground1 = entity_new(app->e_manager, gpos1, gsize);
	entity_add_component(ground1, RENDER_COMPONENT, render_component(app->renderer, gcol1, cord, tex), sizeof(RenderComponentStruct));
	entity_add_component(ground1, BOX_COLLIDER_COMPONENT, box_collider_component(gpos1.x, gpos1.y, gsize.x, gsize.y), sizeof(BoxColliderComponentStruct));

	// Movement
	bool left, right, jump;	
	left = right = jump = false;

	printf("%s\n", glGetString(GL_VERSION));

	while(app->running)
	{
		glnx_calc_frame(app);
		gln_clear_window(app->window, bg);

		// Event loop
		while (SDL_PollEvent(&app->event))
		{
			if (app->event.type == SDL_QUIT)
			{
				app->running = false;
			}
			if (app->event.type == SDL_KEYDOWN)
			{
				switch(app->event.key.keysym.sym)
				{
					case SDLK_SPACE:
						jump = true;
						break;
					case SDLK_a:
						left = true;
						break;
					case SDLK_d:
						right = true;
						break;
				}
			}
			if (app->event.type == SDL_KEYUP)
			{
				switch(app->event.key.keysym.sym)
				{
					case SDLK_a:
						left = false;
						break;
					case SDLK_d:
						right = false;
						break;
					case SDLK_SPACE:
						jump = false;
						break;
				}
			}
		}
		if (left)
			entity_add_force(box, 'X', LEFT);
		if (right)
			entity_add_force(box, 'X', RIGHT);

		if (jump)
		{
			BoxColliderComponentStruct* col = dict_get(box->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT));
			if (col->down_hit)
				entity_add_force(box, 'Y', JUMP);
		}
		
		vec2f offset = { 1, -1 };
		ortho_cam_follow(app->camera, entity_get_gl_pos(box, 800, 600), offset);
		entity_update(app->e_manager, app->renderer, app->dt);
		glnx_update_frame(app);
	}

	glnx_core_app_clean(app);
	return 0;
}
