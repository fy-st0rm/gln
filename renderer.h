#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "includes.h"

typedef struct
{
	vec2f pos;
	vec4f color;
} Vertex;

typedef struct
{
	Vertex v[4];
} Quad;

typedef struct
{
	// Constants
	int max_quad_cnt;
	int max_idx_cnt;
	int max_buff_size;
	int max_texture;

	// Buffer IDs
	unsigned int VAO, VBO, IBO;

	// Buffer
	int buff_idx;
	float* quad_buffer;

} GLNRenderer;

GLNRenderer* gln_create_renderer	(int max_quad_cnt);
void		 gln_destroy_renderer	(GLNRenderer* renderer);
void		 gln_init_renderer		(GLNRenderer* renderer);
void		 gln_render_begin 		(GLNRenderer* renderer);
void		 gln_render_end			(GLNRenderer* renderer);

Quad*		 gln_create_quad		(float x, float y, float w, float h, vec4f color);
void		 gln_push_quad			(GLNRenderer* renderer, Quad* quad);

#endif
