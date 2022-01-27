#ifndef _SHADER_H_
#define _SHADER_H_

#include "includes.h"

unsigned int create_shader (char* vertex_shader, char* fragment_shader);
unsigned int compile_shader(unsigned int type, char* shader_src);

#endif
