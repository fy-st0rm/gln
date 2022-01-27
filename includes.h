#ifndef _INCLUDES_H_
#define _INCLUDES_H_

// Standard C lib
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Graphics lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Math
#include "la.h"

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

#endif
