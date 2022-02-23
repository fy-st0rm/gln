#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "includes.h"

typedef struct
{
	// Constants
	int max_quad_cnt;
	int max_idx_cnt;
	int max_buff_size;
	int max_texture;

	int white_texture;
	int tex_len;

	// Buffer IDs
	unsigned int VAO, VBO, IBO;

	// Buffer
	int buff_idx;
	float* quad_buffer;
	unsigned int* texture_slots;

} GLNRenderer;

GLNRenderer* gln_create_renderer	(int max_quad_cnt);
void		 gln_destroy_renderer	(GLNRenderer* renderer);
void		 gln_init_renderer		(GLNRenderer* renderer);
void		 gln_render_begin 		(GLNRenderer* renderer);
void		 gln_render_end			(GLNRenderer* renderer);

Quad*		 gln_create_quad		(GLNRenderer* renderer, vec3f pos, vec2f size, vec4f color, vec4f tex_cord, int tex_id);
void		 gln_destroy_quad		(Quad* quad);
void		 gln_push_quad			(GLNRenderer* renderer, Quad* quad);

#endif
