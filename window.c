#include "window.h"

GLNWindow* gln_create_window(const char* title, int w, int h)
{
	GLNWindow* window = malloc(sizeof(GLNWindow));
	window->width = w;
	window->height = h;

	if (!glfwInit())
	{
		fprintf(stderr, "[Error]: Failed to initialize glfw.\n");
		exit(1);
	}
	
	// Creating glfw window hints 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating the actual window
	// TODO: Handle the window resizalbe 
	window->window = glfwCreateWindow(w, h, title, NULL, NULL);

	glfwMakeContextCurrent(window->window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "[Error]: Failed to initialize glew.\n");
		exit(1);
	}
	return window;
}

void gln_destroy_window(GLNWindow* window)
{
	free(window);
	glfwTerminate();
}

void gln_clear_window(GLNWindow* window, vec4f color)
{
	GLCall(glClearColor(color.x, color.y, color.z, color.w));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

bool gln_window_running(GLNWindow* window)
{
	return !glfwWindowShouldClose(window->window);
}

void gln_update_window(GLNWindow* window)
{
	glfwSwapBuffers(window->window);
	glfwPollEvents();
}
