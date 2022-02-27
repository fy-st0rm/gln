#ifndef _LA_H_
#define _LA_H_

// TODO: Add all the arthimetic operations of these vectors
// Vectors
typedef struct { float x, y;       } vec2f;
typedef struct { float x, y, z;    } vec3f;
typedef struct { float x, y, z, w; } vec4f;

// Matrix
typedef struct { float m[4][4]; } mat4f;

typedef struct
{
	vec3f pos;
	vec4f color;
	vec2f tex_cord;
	float tex_id;
} Vertex;

typedef struct
{
	Vertex v[4];
} Quad;


// Print functions
void print_mat4f(mat4f* m);
void print_vec3f(vec3f* v);
void print_vec4f(vec4f* v);

void mat4f_mat4f_mul	(mat4f* out, mat4f m1, mat4f m2);
void mat4f_identity		(mat4f* out);
void mat4f_enlarge      (mat4f* out, float scale);
void mat4f_inverse		(mat4f* out, mat4f in);
void mat4f_translate	(mat4f* out, vec3f v);
void mat4f_vec3_mul		(vec3f* out, mat4f m, vec3f v);
void mat4f_quad_mul     (mat4f m, Quad* quad);
void ortho_proj			(mat4f* out, float left, float right, float bottom, float top, float far, float near);

// Rotation matrices
void rotation_x(mat4f* out, float theta);
void rotation_y(mat4f* out, float theta);
void rotation_z(mat4f* out, float theta);

#endif
