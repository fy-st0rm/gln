#include "glnx.h"

#define MASS 1
#define GRAVITY 5
#define FRICTION 0.2

// Movement constant
#define LEFT -34
#define RIGHT 34
#define JUMP -54

// Animation speed
#define IDLE_SPEED 10.0f
#define WALK_SPEED 9.0f

#define IDLE_LEFT "idle_left"
#define IDLE_RIGHT "idle_right"
#define WALK_LEFT "walk_left"
#define WALK_RIGHT "walk_right"

#define SPRITE_WIDTH  11.0f
#define SPRITE_HEIGHT 2.0F

void player_init_frames(Entity* player)
{
	AnimationComponentStruct* animation = entity_get_component(player, ANIMATION_COMPONENT);

	// TODO: Lists are now freed here make sure to free it
	// IDLE SPRITE CORDS
	vec4f idle_down_1  = {0.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_2  = {1.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_3  = {2.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_4  = {3.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_5  = {4.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_6  = {5.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_7  = {6.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_8  = {7.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_9  = {8.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_10 = {9.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f idle_down_11 = {10.0f / SPRITE_WIDTH, 0.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};

	List* list_1 = list_new();
	list_append(list_1, (void*) &idle_down_1, sizeof(idle_down_1));
	list_append(list_1, (void*) &idle_down_2, sizeof(idle_down_2));
	list_append(list_1, (void*) &idle_down_3, sizeof(idle_down_3));
	list_append(list_1, (void*) &idle_down_4, sizeof(idle_down_4));
	list_append(list_1, (void*) &idle_down_5, sizeof(idle_down_5));
	list_append(list_1, (void*) &idle_down_6, sizeof(idle_down_6));
	list_append(list_1, (void*) &idle_down_7, sizeof(idle_down_7));
	list_append(list_1, (void*) &idle_down_8, sizeof(idle_down_8));
	list_append(list_1, (void*) &idle_down_9, sizeof(idle_down_9));
	list_append(list_1, (void*) &idle_down_10, sizeof(idle_down_10));
	list_append(list_1, (void*) &idle_down_11, sizeof(idle_down_11));
	animation_add_frame(animation, IDLE_RIGHT, list_1);

	// Walk sprite coords
	vec4f walk_down_1 = {0.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f walk_down_2 = {1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f walk_down_3 = {2.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f walk_down_4 = {3.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f walk_down_5 = {4.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};
	vec4f walk_down_6 = {5.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT, 1.0f / SPRITE_WIDTH, 1.0f / SPRITE_HEIGHT};

	List* list_3 = list_new();
	list_append(list_3, (void*) &walk_down_1, sizeof(walk_down_1));
	list_append(list_3, (void*) &walk_down_2, sizeof(walk_down_2));
	list_append(list_3, (void*) &walk_down_3, sizeof(walk_down_3));
	list_append(list_3, (void*) &walk_down_4, sizeof(walk_down_4));
	list_append(list_3, (void*) &walk_down_5, sizeof(walk_down_5));
	list_append(list_3, (void*) &walk_down_6, sizeof(walk_down_6));
	animation_add_frame(animation, WALK_RIGHT, list_3);
}

int main(int argc, char** argv)
{
	int fps = 60;
	GLNXCoreApp* app = glnx_core_app_new("Anime test", 800, 600, 800, 600, 1000, fps);
	vec4f bg = { 0.5f, 0.5f, 0.5f, 1.0f };

	// Player
	vec3f pos      = { 100.0f, 100.0f, 0.0f };
	vec2f size     = { 64.0f, 128.0f };
	vec2f col_size = { 64.0f, 111.0f };
	vec4f color    = { 1.0f, 1.0f, 1.0f, 1.0f };
	vec4f cord     = { 0.0f, 0.0f , 1.0f, 1.0f };
	GLNTexture tex = gln_load_texture(app->window, "player.png");

	Entity* player = entity_new(app->e_manager, pos, size);
	entity_add_component(player, RENDER_COMPONENT, render_component(app->renderer, color, cord, tex), sizeof(RenderComponentStruct));
	entity_add_component(player, BOX_COLLIDER_COMPONENT, box_collider_component(pos.x + 5, pos.y, col_size.x, col_size.y), sizeof(BoxColliderComponentStruct));
	entity_add_component(player, PHYSICS_COMPONENT, physics_component(MASS, FRICTION, GRAVITY, true, true), sizeof(PhysicsComponentStruct));
	entity_add_component(player, ANIMATION_COMPONENT, animation_component(IDLE_RIGHT, IDLE_SPEED), sizeof(AnimationComponentStruct));

	player_init_frames(player);

	// Floors
	vec2f gsize     = { 1000.0f, 50.0f };
	vec4f gcol      = { 0.0f, 0.5f, 0.1f, 1.0f };
	GLNTexture tex2 = { 0.0f, 0.0f, 0.0f };

	int cnt = 9999;
	float x = 0.0f;
	for (int i = 0; i < cnt; i++)
	{
		vec3f gpos      = { x, 300.0f, 0.0f };
		Entity* ground  = entity_new(app->e_manager, gpos, gsize);
		entity_add_component(ground, RENDER_COMPONENT, render_component(app->renderer, gcol, cord, tex2), sizeof(RenderComponentStruct));
		entity_add_component(ground, BOX_COLLIDER_COMPONENT, box_collider_component(gpos.x, gpos.y, gsize.x, gsize.y), sizeof(BoxColliderComponentStruct));
		entity_add_component(ground, PHYSICS_COMPONENT, physics_component(MASS, FRICTION, GRAVITY, false, false), sizeof(PhysicsComponentStruct));
		x += 50.0f;
	}

	// Movement
	bool left, right, jump, up, down;
	left = right = jump = false;
	up = down = false;

	while (app->running)
	{
		glnx_calc_frame(app);
		gln_clear_window(app->window, bg);

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
					case SDLK_a:
						left = true;
						break;
					case SDLK_d:
						right = true;
						break;
					case SDLK_w:
						up = true;
						break;
					case SDLK_s:
						down = true;
						break;
					case SDLK_SPACE:
						jump = true;
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
					case SDLK_w:
						up = false;
						break;
					case SDLK_s:
						down = false;
						break;
				}
			}
		}
		if (left)
		{
			entity_add_force(player, 'X', LEFT);
			player->ang = 2.8f;
		}
		if (right)
		{
			entity_add_force(player, 'X', RIGHT);
			player->ang = 0.0f;
		}
		/*
		if (up)
			entity_add_force(player, 'Y', LEFT);
		if (down)
			entity_add_force(player, 'Y', RIGHT);
			*/

		BoxColliderComponentStruct* col = dict_get(player->components, BOX_COLLIDER_COMPONENT, sizeof(char) * strlen(BOX_COLLIDER_COMPONENT));
		if (jump)
		{
			if (col->down_hit)
				entity_add_force(player, 'Y', JUMP);
		}

		// Animation
		AnimationComponentStruct* animation = entity_get_component(player, ANIMATION_COMPONENT);
		RenderComponentStruct*    ren_comp  = entity_get_component(player, RENDER_COMPONENT);
		PhysicsComponentStruct* phy_comp = entity_get_component(player, PHYSICS_COMPONENT);

		// Setting animation states
		animation_set_state(animation, WALK_RIGHT, WALK_SPEED, phy_comp->velocity.x != 0.0f);
		animation_set_state(animation, IDLE_RIGHT, IDLE_SPEED, phy_comp->velocity.x == 0.0f);

		vec4f* tex = animation_get_frame(animation);
		ren_comp->tex_cord = *tex;

		vec2f offset = { 1, -1 };
		ortho_cam_follow(app->camera, entity_get_gl_pos(player, 800, 600), offset);
		entity_update(app->e_manager, app->renderer, app->dt);
		glnx_update_frame(app);
	}
	glnx_core_app_clean(app);
	return 0;
}
