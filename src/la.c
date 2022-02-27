#include "../includes/la.h" 
#include <math.h>
#include <stdio.h>

void print_mat4f(mat4f* m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f\t", m->m[i][j]);
		printf("\n");
	}
	printf("\n");
}

void print_vec3f(vec3f* v)
{
	printf("%f %f %f\n", v->x, v->y, v->z);
}

void print_vec4f(vec4f* v)
{
	printf("%f %f %f %f\n", v->x, v->y, v->z, v->w);
}

void mat4f_mat4f_mul(mat4f* out, mat4f m1, mat4f m2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			out->m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				out->m[i][j] += m1.m[i][k] * m2.m[k][j];
			}	
		}
	}
}

void mat4f_identity(mat4f* out)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out->m[i][j] = 0.0f;

	out->m[0][0] = 1.0f;
	out->m[1][1] = 1.0f;
	out->m[2][2] = 1.0f;
	out->m[3][3] = 1.0f;
}

void mat4f_enlarge(mat4f* out, float scale)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out->m[i][j] = 0.0f;

	out->m[0][0] = scale;
	out->m[1][1] = scale;
	out->m[2][2] = scale;
}

void mat4f_inverse(mat4f* out, mat4f in)
{
	// TEMP ig
	out->m[0][0] = in.m[0][0]; 
	out->m[0][1] = in.m[1][0]; 
	out->m[0][2] = in.m[2][0]; 
	out->m[0][3] = 0.0f;
	out->m[1][0] = in.m[0][1]; 
	out->m[1][1] = in.m[1][1];
   	out->m[1][2] = in.m[2][1];
   	out->m[1][3] = 0.0f;
	out->m[2][0] = in.m[0][2];
   	out->m[2][1] = in.m[1][2];
   	out->m[2][2] = in.m[2][2];
   	out->m[2][3] = 0.0f;
	out->m[3][0] = -(in.m[3][0] * out->m[0][0] + in.m[3][1] * out->m[1][0] + in.m[3][2] * out->m[2][0]);
	out->m[3][1] = -(in.m[3][0] * out->m[0][1] + in.m[3][1] * out->m[1][1] + in.m[3][2] * out->m[2][1]);
	out->m[3][2] = -(in.m[3][0] * out->m[0][2] + in.m[3][1] * out->m[1][2] + in.m[3][2] * out->m[2][2]);
	out->m[3][3] = 1.0f;
}

void mat4f_translate(mat4f* out, vec3f v)
{
	out->m[0][0] = 1.0f;
	out->m[1][1] = 1.0f;
	out->m[2][2] = 1.0f;
	out->m[3][3] = 1.0f;
	out->m[3][0] = v.x;
	out->m[3][1] = v.y;
	out->m[3][2] = v.z;
}

void mat4f_vec3_mul(vec3f* out, mat4f m, vec3f v)
{
	out->x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	out->y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	out->z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	if (w)
	{
		out->x /= w;
		out->y /= w;
		out->z /= w;
	}
}

void mat4f_quad_mul(mat4f m, Quad* quad)
{
	vec3f p1 = quad->v[0].pos;
	vec3f p2 = quad->v[1].pos;
	vec3f p3 = quad->v[2].pos;
	vec3f p4 = quad->v[3].pos;
	mat4f_vec3_mul(&quad->v[0].pos, m, p1);
	mat4f_vec3_mul(&quad->v[1].pos, m, p2);
	mat4f_vec3_mul(&quad->v[2].pos, m, p3);
	mat4f_vec3_mul(&quad->v[3].pos, m, p4);
}

void ortho_proj(mat4f* out, float left, float right, float bottom, float top, float far, float near)
{
	out->m[0][0] = 2 / (right - left); 
    out->m[0][1] = 0; 
    out->m[0][2] = 0; 
    out->m[0][3] = 0; 
 
    out->m[1][0] = 0; 
    out->m[1][1] = 2 / (top - bottom); 
    out->m[1][2] = 0; 
    out->m[1][3] = 0; 
 
    out->m[2][0] = 0; 
    out->m[2][1] = 0; 
    out->m[2][2] = -2 / (far - near); 
    out->m[2][3] = 0; 
 
    out->m[3][0] = -(right + left) / (right - left); 
    out->m[3][1] = -(top + bottom) / (top - bottom); 
    out->m[3][2] = -(far + near) / (far - near); 
    out->m[3][3] = 1; 
}

void rotation_y(mat4f* out, float theta)
{
	out->m[0][0] = cosf(theta);
	out->m[0][2] = sinf(theta);
	out->m[2][0] = -sinf(theta);
	out->m[1][1] = 1.0f;
	out->m[2][2] = cosf(theta);
	out->m[3][3] = 1.0f;
}

void rotation_x(mat4f* out, float theta)
{
	out->m[0][0] = 1.0f;
	out->m[1][1] = cosf(theta);
	out->m[1][2] = sinf(theta);
	out->m[2][1] = -sinf(theta);
	out->m[2][2] = cosf(theta);
	out->m[3][3] = 1.0f;
}

void rotation_z(mat4f* out, float theta)
{
	out->m[0][0] = cosf(theta);
	out->m[0][1] = sinf(theta);
	out->m[1][0] = -sinf(theta);
	out->m[1][1] = cosf(theta);
	out->m[2][2] = 1.0f;
	out->m[3][3] = 1.0f;
}
