#include "../includes/renderer.h"

GLNRenderer* gln_create_renderer(int max_quad_cnt)
{
	GLNRenderer* renderer = malloc(sizeof(GLNRenderer));
	renderer->max_quad_cnt = max_quad_cnt;
	renderer->max_idx_cnt = max_quad_cnt * 6;
	renderer->max_buff_size = max_quad_cnt * sizeof(Vertex) * 4;
	renderer->max_texture = 32;
	renderer->white_texture = 0;
	renderer->tex_len = 0;

	renderer->buff_idx = 0;
	renderer->quad_buffer = malloc(renderer->max_buff_size);
	renderer->texture_slots = calloc(renderer->max_texture, sizeof(unsigned int));
	return renderer;
}

void gln_destroy_renderer(GLNRenderer* renderer)
{
	glDeleteVertexArrays(1, &renderer->VAO);
	glDeleteBuffers(1, &renderer->VBO);
	glDeleteBuffers(1, &renderer->IBO);

	free(renderer->quad_buffer);
	free(renderer);
}

void gln_init_renderer(GLNRenderer* renderer)
{
	// Vertex arrays
	GLCall(glGenVertexArrays(1, &renderer->VAO));
	GLCall(glBindVertexArray(renderer->VAO));

	// Vertex buffer
	GLCall(glGenBuffers(1, &renderer->VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, renderer->max_buff_size, NULL, GL_DYNAMIC_DRAW));

	// Generating Vertex attributes
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color)));

	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, tex_cord)));

	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, tex_id)));

	// Index buffer
	unsigned int indices[renderer->max_idx_cnt];

	// Generating indices
	for (int offset = 0, i = 0; i < renderer->max_idx_cnt; i += 6) 
	{
		indices[0 + i] = 0 + offset;
		indices[1 + i] = 1 + offset;
		indices[2 + i] = 2 + offset;

		indices[3 + i] = 2 + offset;
		indices[4 + i] = 3 + offset;
		indices[5 + i] = 0 + offset;
		offset += 4;
	}

	// Creating index buffer
	GLCall(glGenBuffers(1, &renderer->IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	
	// Generating white texture as a default texture
	unsigned int white = 0xffffffff;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white));
	renderer->texture_slots[0] = renderer->white_texture;
	renderer->tex_len++;
}

void gln_render_begin(GLNRenderer* renderer)
{
	renderer->buff_idx = 0;
	free(renderer->quad_buffer);
	renderer->quad_buffer = malloc(renderer->max_buff_size);
}

void gln_render_end(GLNRenderer* renderer)
{
	/*
	int idx = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			printf("%f ", renderer->quad_buffer[idx]);
			idx++;
		}
		printf("\n");
	}
	*/

	// Bind the textures
	for (int i = 0; i < renderer->tex_len; i++)
	{
		GLCall(glBindTextureUnit(i, renderer->texture_slots[i]));
	}

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->max_buff_size, renderer->quad_buffer));

	GLCall(glBindVertexArray(renderer->VAO));
	GLCall(glDrawElements(GL_TRIANGLES, renderer->buff_idx, GL_UNSIGNED_INT, NULL));
}

Quad* gln_create_quad (GLNRenderer* renderer, vec3f pos, vec2f size, vec4f color, vec4f tex_cord, int tex_id)
{
	int id = -1;
	for (int i = 0; i < renderer->tex_len; i++)
	{
		if (renderer->texture_slots[i] == tex_id)
			id = i;
	}
	if (id == -1)
	{
		renderer->texture_slots[renderer->tex_len] = tex_id;
		id = renderer->tex_len;
		renderer->tex_len++;
	}

	Quad* quad = malloc(sizeof(Quad));
	
	quad->v[0].pos.x = pos.x;
	quad->v[0].pos.y = pos.y;
	quad->v[0].pos.z = pos.z;

	quad->v[1].pos.x = pos.x + size.x;
	quad->v[1].pos.y = pos.y;
	quad->v[1].pos.z = pos.z;

	quad->v[2].pos.x = pos.x + size.x; 
	quad->v[2].pos.y = pos.y + size.y; 
	quad->v[2].pos.z = pos.z;
	
	quad->v[3].pos.x = pos.x;
	quad->v[3].pos.y = pos.y + size.y;
	quad->v[3].pos.z = pos.z;

	quad->v[0].color = color;
	quad->v[1].color = color;
	quad->v[2].color = color;
	quad->v[3].color = color;

	quad->v[0].tex_cord.x = tex_cord.x;
	quad->v[0].tex_cord.y = tex_cord.y;
	quad->v[1].tex_cord.x = tex_cord.x + tex_cord.z;
	quad->v[1].tex_cord.y = tex_cord.y;
	quad->v[2].tex_cord.x = tex_cord.x + tex_cord.z;
	quad->v[2].tex_cord.y = tex_cord.y + tex_cord.w;
	quad->v[3].tex_cord.x = tex_cord.x;
	quad->v[3].tex_cord.y = tex_cord.y + tex_cord.w;

	quad->v[0].tex_id = id;
	quad->v[1].tex_id = id;
	quad->v[2].tex_id = id;
	quad->v[3].tex_id = id;

	return quad;
}

void gln_destroy_quad(Quad* quad)
{
	free(quad);
}

void gln_push_quad(GLNRenderer* renderer, Quad* quad)
{
	for (int i = 0; i < 4; i++)
	{
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].pos.x;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].pos.y;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].pos.z;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.x;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.y;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.z;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.w;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].tex_cord.x;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].tex_cord.y;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].tex_id;
	}
}

