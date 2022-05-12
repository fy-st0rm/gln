#ifndef _GLNX_CORE_H_
#define _GLNX_CORE_H_

#include "../gln.h"
#include "entity.h"

/*
 * Default vertex shader for glnx
 */

static char* glnx_vertex_shader = "#version 460 core\n"
"\n"
"layout(location = 0) in vec4 in_position; \n "
"layout(location = 1) in vec4 in_color; 	  \n "
"layout(location = 2) in vec2 in_tex_cord; \n "
"layout(location = 3) in float in_tex_id;  \n"
"\n "
"uniform mat4 mvp;\n"
"out vec4 out_color;\n  "
"out vec2 out_tex_cord;\n  "
"out float out_tex_id;\n  "
"void main()\n  "
"{\n  "
"    out_color = in_color;\n"
"    out_tex_cord = in_tex_cord;\n"
"    out_tex_id = in_tex_id;\n"
"	gl_Position = mvp * in_position;\n"
"};\n";



/*
 * Default fragment shader for glnx
 */

static char* glnx_fragment_shader = "#version 460 core\n"
"layout(location = 0) out vec4 color;\n"
"\n"
"in vec4  out_color;\n"
"in vec2  out_tex_cord;\n "
"in float out_tex_id;\n"
"\n"
"uniform sampler2D textures[32];\n"
"\n"
"void main()\n"
"{\n"
"    int index = int(out_tex_id);\n"
"    color = texture(textures[index], out_tex_cord) * out_color;\n"
"};\n";

/*
 * GLNX Core application
 * - Holds window, renderer and shaders
 * - Handles the cameras 
 * - Creates a buffer for entities;
 */

// TODO: [ ] Add physics system

typedef struct
{
	GLNWindow* window;
	GLNRenderer* renderer;
	SDL_Event event;
	unsigned int shader;

	// Entity Manager
	EntityManager* e_manager;

	// Camera
	Ortho_camera* camera;

	// Loop flag
	bool running;

	// Frames per second
	int      fps;
	int      frame_delay;
	uint32_t frame_start;
	int      frame_time;

	// Delta times
	float    dt;

} GLNXCoreApp;

GLNXCoreApp* glnx_core_app_new(char* win_title, int win_width, int win_height, int cam_width, int cam_height, int max_quad_cnt, int fps);
void		 glnx_core_app_clean(GLNXCoreApp* app);
void 		 glnx_init(GLNXCoreApp* app);
void 		 glnx_calc_frame(GLNXCoreApp* app);
void 		 glnx_update_frame(GLNXCoreApp* app);

#endif
