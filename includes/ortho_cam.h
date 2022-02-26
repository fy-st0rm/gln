#ifndef _ORTHO_CAM_H_
#define _ORTHO_CAM_H_

#include "includes.h"

typedef struct
{
	vec3f* pos;
	mat4f* proj;
} Ortho_camera;

Ortho_camera* ortho_cam_new(vec3f pos, float left, float right, float bottom, float top, float far, float near);
void  		  ortho_cam_update(Ortho_camera* camera, unsigned int shader);
void		  ortho_cam_destryo(Ortho_camera* camera);

#endif
