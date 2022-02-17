#ifndef _SHADER_H_
#define _SHADER_H_

#include "includes.h"

char*		 load_file         (char* file);
unsigned int gln_load_shader   (char* vertex_shader_file, char* fragment_shader_file);
unsigned int gln_compile_shader(unsigned int type, char* shader_src);

#endif
