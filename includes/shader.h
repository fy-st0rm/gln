#ifndef _SHADER_H_
#define _SHADER_H_

#include "includes.h"

unsigned int gln_load_shader         (char* vertex_shader_file, char* fragment_shader_file);
unsigned int gln_load_shader_from_src(char* vertex_shader, char* fragment_shader);
unsigned int gln_compile_shader      (unsigned int type, char* shader_src);

#endif
