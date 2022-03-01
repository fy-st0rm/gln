#ifndef _ORTHO_CAM_H_
#define _ORTHO_CAM_H_

#include "includes.h"

typedef struct
{
	vec3f* origin;
	vec3f* pos;
	mat4f* proj;
	mat4f* mvp;
	float  width, height;
} Ortho_camera;

Ortho_camera* ortho_cam_new(vec3f pos, float left, float right, float bottom, float top, float far, float near);
void  		  ortho_cam_update(Ortho_camera* camera);
void  		  ortho_cam_update_shader(Ortho_camera* camera, unsigned int shader);
void		  ortho_cam_follow(Ortho_camera* camera, vec3f pos, vec2f offset);
void		  ortho_cam_destroy(Ortho_camera* camera);

#endif
