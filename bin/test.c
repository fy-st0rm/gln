#include "../gln.h"

// Writing shader
char* vertex_shader = 
	"#version 330 core\n"
	"layout(location = 0) in vec4 in_position;\n"
	"layout(location = 1) in vec4 in_color;\n"
	"\n"
	"out vec4 out_color;\n"
	"void main()\n"
	"{\n"
	"gl_Position = in_position;\n"
	"out_color = in_color;\n"
	"}";

char* fragment_shader = 
	"#version 330 core\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"in vec4 out_color;\n"
	"void main()\n"
	"{\n"
	"color = out_color;\n"
	"}";


int main()
{
	GLNWindow* window = gln_create_window("Window", 800, 600);
	GLNRenderer* renderer = gln_create_renderer(1000);
	gln_init_renderer(renderer);

	vec4f color = { 1.0, 1.0, 0.0, 1.0 };
	Quad* box = gln_create_quad(-0.5, -0.5, 0.5, 0.5, color);
	unsigned int shader = create_shader(vertex_shader, fragment_shader);

	vec4f bg = { 0.0, 0.0, 0.0, 1.0 };
	glViewport(0, 0, 800, 600);
	while (gln_window_running(window))
   	{
		gln_clear_window(window, bg); 

		gln_render_begin(renderer);
		gln_push_quad(renderer, box);

		glUseProgram(shader);
		gln_render_end(renderer);

		gln_update_window(window);
	}

	gln_destroy_renderer(renderer);
	gln_destroy_window(window);
	return 0;
}
