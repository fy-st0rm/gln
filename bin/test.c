#include "../includes/gln.h"

int main(int argc, char** argv)
{
	int width = 800, height = 600;
	GLNWindow* window = gln_create_window("Window", width, height);
	GLNRenderer* renderer = gln_create_renderer(1000);
	unsigned int shader = gln_load_shader("shader.vert", "shader.frag");

	gln_init_renderer(renderer);

	printf("Opengl Version: %s\n", glGetString(GL_VERSION));
	// Generating samplers for the shader 
	int samplers[renderer->max_texture];
	for (int i = 0; i < renderer->max_texture; i++)
	{
		samplers[i] = i;
	}

	// Providing samplers to the shader
	glUseProgram(shader);
	int loc = glGetUniformLocation(shader, "textures");
	GLCall(glUniform1iv(loc, renderer->max_texture, samplers));

	// Player stuff
	GLNTexture texture = gln_load_texture(window, "player.png");
	int sprite_cnt = 6;
	vec3f pos = { 400.0f, 300.0f, 0.0f };
	vec2f size = { 64.0f, 64.0f };
	vec4f color = {  1.0,  1.0, 1.0, 1.0 };
	
	vec4f walk0 = { 0.0 / sprite_cnt, 0.0, 1.0 / sprite_cnt, 1.0 };
	vec4f walk1 = { 1.0 / sprite_cnt, 0.0, 1.0 / sprite_cnt, 1.0 };
	vec4f walk2 = { 2.0 / sprite_cnt, 0.0, 1.0 / sprite_cnt, 1.0 };
	vec4f walk3 = { 3.0 / sprite_cnt, 0.0, 1.0 / sprite_cnt, 1.0 };
	vec4f walk4 = { 4.0 / sprite_cnt, 0.0, 1.0 / sprite_cnt, 1.0 };
	vec4f walk5 = { 5.0 / sprite_cnt, 0.0, 1.0 / sprite_cnt, 1.0 };
	Dict* anime = dict_new(sprite_cnt);
	dict_insert(anime, (void*)1, (void*)&walk0); 
	dict_insert(anime, (void*)2, (void*)&walk1); 
	dict_insert(anime, (void*)3, (void*)&walk2); 
	dict_insert(anime, (void*)4, (void*)&walk3); 
	dict_insert(anime, (void*)5, (void*)&walk4); 
	dict_insert(anime, (void*)6, (void*)&walk5); 
	dict_print(anime);
	float curr_frame = 1.0;

	// Cameras
	float cam_speed = 0.05f;
	vec3f cam_pos = { 0.0f, 0.0f, 0.0f };
	Ortho_camera* camera = ortho_cam_new(cam_pos, 0.0f, width, height, 0.0f, 1000.0f, -1.0f);

	vec4f bg = { 1.0, 1.0, 0.0, 1.0 };
	glViewport(0, 0, 800, 600);

	// Frame stuff
	int fps = 60;
	int frame_delay = 1000 / fps;
	Uint32 frame_start;
	int frame_time;

	bool running = true;
	SDL_Event event;
	while (running)
   	{
		frame_start = SDL_GetTicks();
		gln_clear_window(window, bg); 

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
						camera->pos->x += cam_speed;
						break;
					case SDLK_RIGHT:
						camera->pos->x -= cam_speed;
						break;
					case SDLK_UP:
						camera->pos->y -= cam_speed;
						break;
					case SDLK_DOWN:
						camera->pos->y += cam_speed;
						break;
				}
			}
		}

		glUseProgram(shader);
		
		ortho_cam_update(camera, shader);

		vec4f* tex = dict_get(anime, (int)curr_frame);
		Quad* quad  = gln_create_quad(renderer, pos, size, color, *tex, texture.id);

		gln_render_begin(renderer);
		gln_push_quad(renderer, quad);
		gln_render_end(renderer);
		gln_update_window(window);

		gln_destroy_quad(quad);

		curr_frame += 0.05;
		if (curr_frame > 7.0) curr_frame = 1.0;

		frame_time = SDL_GetTicks() - frame_start;
		if (frame_delay > frame_time)
		{
			SDL_Delay(frame_delay - frame_time);		
		}
	}

	gln_destroy_renderer(renderer);
	gln_destroy_window(window);
	return 0;
}
