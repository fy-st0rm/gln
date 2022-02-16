#include "../includes/gln.h"

//TODO: Fix the shader to use the given texture id

// Writing shader
char* vertex_shader = 
	"#version 330 core\n"
	"layout(location = 0) in vec4 in_position;\n"
	"layout(location = 1) in vec4 in_color;\n"
	"layout(location = 2) in vec2 in_tex_cord;\n"
	"layout(location = 3) in float in_tex_id;\n"
	"\n"
	"out vec4 out_color;\n"
	"out vec2 out_tex_cord;\n"
	"out float out_tex_id;\n"
	"void main()\n"
	"{\n"
	"out_color = in_color;\n"
	"out_tex_cord = in_tex_cord;\n"
	"out_tex_id = in_tex_id;\n"
	"gl_Position = in_position;\n"
	"}";

char* fragment_shader = 
	"#version 330 core\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"in vec4  out_color;\n"
	"in vec2  out_tex_cord;\n"
	"in float out_tex_id;\n"
	"\n"
	"uniform sampler2D textures[32];\n"
	"\n"
	"void main()\n"
	"{\n"
	"int index = int(out_tex_id);\n"
	"color = texture(textures[1], out_tex_cord) * out_color;\n"
	"}";


int main()
{
	GLNWindow* window = gln_create_window("Window", 800, 600);
	GLNRenderer* renderer = gln_create_renderer(1000);
	unsigned int shader = create_shader(vertex_shader, fragment_shader);

	gln_init_renderer(renderer);

	printf("Opengl Version: %s\n", glGetString(GL_VERSION));

	int samplers[renderer->max_texture];
	for (int i = 0; i < renderer->max_texture; i++)
	{
		samplers[i] = i;
	}

	glUseProgram(shader);
	int loc = glGetUniformLocation(shader, "textures");
	GLCall(glUniform1iv(loc, renderer->max_texture, samplers));

	GLNTexture texture = gln_load_texture(window, "ship.png");

	vec2f pos  = { -1.0, -0.5 };
	vec2f size = { 1.0, 1.0 };
	vec4f color = { 1.0, 1.0, 1.0, 1.0 };
	Object* obj = gln_create_object(renderer, pos, size, color, texture.id);

	vec4f bg = { 1.0, 1.0, 1.0, 1.0 };
	glViewport(0, 0, 800, 600);

	bool running = true;
	SDL_Event event;
	while (running)
   	{
		gln_clear_window(window, bg); 

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
		}

		gln_render_begin(renderer);
		gln_render_object(renderer, obj);

		glUseProgram(shader);
		gln_render_end(renderer);
		gln_update_window(window);
	}

	gln_destroy_renderer(renderer);
	gln_destroy_window(window);
	return 0;
}
