#include "../includes/ortho_cam.h"

Ortho_camera* ortho_cam_new(vec3f pos, float left, float right, float bottom, float top, float far, float near)
{
	Ortho_camera* camera = (Ortho_camera*) malloc(sizeof(Ortho_camera));
	camera->pos  = (vec3f*) malloc(sizeof(vec3f));
	camera->proj = (mat4f*) malloc(sizeof(mat4f));

	camera->pos->x = pos.x;
	camera->pos->y = pos.y;
	camera->pos->z = pos.z;
	ortho_proj(camera->proj, left, right, bottom, top, far, near);

	return camera;
}

void ortho_cam_update(Ortho_camera* camera, unsigned int shader)
{
	mat4f transform = { 0 };
	mat4f view_mat = { 0 };
	mat4f proj_mat = { 0 };
	mat4f mvp = { 0 };

	// Calculating matrix
	mat4f_identity(&transform);
	mat4f_translate(&transform, *camera->pos);
	mat4f_inverse(&view_mat, transform);
	mat4f_mat4f_mul(&mvp, *camera->proj, view_mat);

	int loc = glGetUniformLocation(shader, "mvp");
	if (loc == -1)
	{
		fprintf(stderr, "[Error]: Cannot find uniform `mvp` in the vertex shader.\n");
		exit(1);
	}
	GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp.m[0][0]));
}

void ortho_cam_destroy(Ortho_camera* camera)
{
	free(camera->pos);
	free(camera->proj);
	free(camera);
}
