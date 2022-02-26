#ifndef _INCLUDES_H_
#define _INCLUDES_H_

// Standard C lib
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Graphics lib
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Math
#include "la.h"

#include "util.h"

// Opengl Error handling
#define ASSERT(x) if (!(x)) exit(1);
#define GLCall(x) ClearError(); \
x;\
ASSERT(ErrorLog(#x, __FILE__, __LINE__))

static void ClearError()
{
    while(glGetError());
}

static bool ErrorLog(const char* function, const char* file, int line)
{
	GLenum error;
    while (error = glGetError())
    {
        fprintf(stderr, "[Error code]: %s\n", error);
        fprintf(stderr, "[Opengl error]: %s %s: %d\n", function ,file, line);
        return false;
    }
    return true;
}

// SDL Error checker
static int sdl_check(int result)
{
	if (result < 0)
	{
		fprintf(stderr, "[ERROR]: %s\n", SDL_GetError());
		exit(1);
	}
	return result;
}

static void* sdl_check_ptr(void* result)
{
	if (!result)
	{
		fprintf(stderr, "[PTR ERROR]: %s\n", SDL_GetError());
		exit(1);
	}
	return result;
}

#endif
