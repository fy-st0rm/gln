#include "renderer.h"

GLNRenderer* gln_create_renderer(int max_quad_cnt)
{
	GLNRenderer* renderer = malloc(sizeof(GLNRenderer));
	renderer->max_quad_cnt = max_quad_cnt;
	renderer->max_idx_cnt = max_quad_cnt * 6;
	renderer->max_buff_size = max_quad_cnt * sizeof(Vertex) * 4;
	renderer->max_texture = 32;

	renderer->buff_idx = 0;
	renderer->quad_buffer = malloc(renderer->max_buff_size);
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
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color)));

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
		for (int j = 0; j < 6; j++)
		{
			printf("%f ", renderer->quad_buffer[idx]);
			idx++;
		}
		printf("\n");
	}
	*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->max_buff_size, renderer->quad_buffer));

	GLCall(glBindVertexArray(renderer->VAO));
	GLCall(glDrawElements(GL_TRIANGLES, renderer->buff_idx, GL_UNSIGNED_INT, NULL));
}

Quad* gln_create_quad (float x, float y, float w, float h, vec4f color)
{
	Quad* quad = malloc(sizeof(Quad));
	
	quad->v[0].pos.x = x;
	quad->v[0].pos.y = y;
	quad->v[1].pos.x = x + w;
	quad->v[1].pos.y = y;
	quad->v[2].pos.x = x + w;
	quad->v[2].pos.y = y + h;
	quad->v[3].pos.x = x;
	quad->v[3].pos.y = y + h;

	quad->v[0].color = color;
	quad->v[1].color = color;
	quad->v[2].color = color;
	quad->v[3].color = color;

	return quad;
}

void gln_push_quad(GLNRenderer* renderer, Quad* quad)
{
	for (int i = 0; i < 4; i++)
	{
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].pos.x;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].pos.y;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.x;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.y;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.z;
		renderer->quad_buffer[renderer->buff_idx++] = quad->v[i].color.w;
	}
}

