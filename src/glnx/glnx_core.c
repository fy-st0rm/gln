#include "../../includes/glnx/glnx_core.h"

GLNXCoreApp* glnx_core_app_new(char* win_title, int win_width, int win_height, int cam_width, int cam_height, int max_quad_cnt, int fps)
{
	GLNXCoreApp* app = malloc(sizeof(GLNXCoreApp));
	app->window = gln_create_window(win_title, win_width, win_height);
	app->renderer = gln_create_renderer(max_quad_cnt);
	app->shader = gln_load_shader_from_src(glnx_vertex_shader, glnx_fragment_shader);
	app->running = true;

	// Entity Manager
	app->e_manager = entity_manager_new();

	// Setting up frames
	app->fps = fps;
	app->frame_delay = 1000 / fps;
	app->frame_start = SDL_GetTicks();

	// Setting up delta time
	app->dt  = 0.0f;
	
	// Setting up the camera
	vec3f pos = { 0.0f, 0.0f, 0.0f };
	app->camera  = ortho_cam_new(pos, 0.0f, cam_width, cam_height, 0.0f, 1000.0f, -100.0f); 

	// Initializes the engine stuff
	glnx_init(app);

	return app;
}

void glnx_core_app_clean(GLNXCoreApp* app)
{
	entity_manager_destroy(app->e_manager);
	ortho_cam_destroy(app->camera);
	gln_destroy_renderer(app->renderer);
	gln_destroy_window(app->window);
	free(app);
}

void glnx_init(GLNXCoreApp* app)
{
	gln_init_renderer(app->renderer);

	// Creating samplers
	int samplers[app->renderer->max_texture];
	for (int i = 0; i < app->renderer->max_texture; i++)
		samplers[i] = i;

	// Providing samplers to the shader
	glUseProgram(app->shader);
	int loc = glGetUniformLocation(app->shader, "textures");
	if (loc == -1)
	{
		fprintf(stderr, "[Error]: Cannot find uniform `textures` in the shader.\n");
		exit(1);
	}
	GLCall(glUniform1iv(loc, app->renderer->max_texture, samplers));
	
	// Setting up gl view port
	glViewport(0, 0, app->window->width, app->window->height);
}

void glnx_calc_frame(GLNXCoreApp* app)
{
	app->dt = (SDL_GetTicks() - app->frame_start) / 1000.f;
	app->frame_start = SDL_GetTicks();
}

void glnx_update_frame(GLNXCoreApp* app)
{
	// Updating the camera
	ortho_cam_update(app->camera);
	ortho_cam_update_shader(app->camera, app->shader);

	// Caping the frame
	app->frame_time = SDL_GetTicks() - app->frame_start;
	if (app->frame_delay > app->frame_time)
	{
		SDL_Delay(app->frame_delay - app->frame_time);		
	}

	// Updating window
	gln_update_window(app->window);
}

