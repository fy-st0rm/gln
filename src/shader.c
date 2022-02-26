#include "../includes/shader.h"

unsigned int gln_load_shader(char* vertex_shader_file, char* fragment_shader_file)
{
	char* vertex_shader   = load_file(vertex_shader_file);
	char* fragment_shader = load_file(fragment_shader_file);

	unsigned int program = glCreateProgram();

	// Compiling vertex and fragment shader
	unsigned int vs = gln_compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = gln_compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	// Attaching shader
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	free(vertex_shader);
	free(fragment_shader);

	return program;
}

unsigned int gln_compile_shader(unsigned int type, char* shader_src)
{
	unsigned int id = glCreateShader(type);
	GLCall(glShaderSource(id, 1, &shader_src, NULL));
	GLCall(glCompileShader(id));

	// Checking error in shader
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*) calloc(length, sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		fprintf(stderr, "Failed to compile [%s shader]\n", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));
		fprintf(stderr, "%s\n", message);

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

