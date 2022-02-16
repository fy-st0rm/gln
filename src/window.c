#include "../includes/window.h"

GLNWindow* gln_create_window(const char* title, int w, int h)
{
	GLNWindow* window = malloc(sizeof(GLNWindow));
	window->width = w;
	window->height = h;

	// SDL init stuff
	sdl_check(SDL_Init(SDL_INIT_EVERYTHING));
	sdl_check(IMG_Init(IMG_INIT_PNG));

	// Creating the actual window
	// TODO: Handle the window resizalbe 
	window->window = sdl_check_ptr(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL));
	window->renderer = sdl_check_ptr(SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED));

	// Setting some attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
   	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	sdl_check_ptr(SDL_GL_CreateContext(window->window));

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "[Error]: Failed to initialize glew.\n");
		exit(1);
	}

	// Setting up alpha channels and blendings
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	return window;
}

void gln_destroy_window(GLNWindow* window)
{
	SDL_DestroyWindow(window->window);
	SDL_DestroyRenderer(window->renderer);
	free(window);
	SDL_Quit();
}

void gln_clear_window(GLNWindow* window, vec4f color)
{
	GLCall(glClearColor(color.x, color.y, color.z, color.w));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void gln_update_window(GLNWindow* window)
{
	SDL_GL_SwapWindow(window->window);
}

