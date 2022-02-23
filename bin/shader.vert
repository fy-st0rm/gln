#version 460 core  

layout(location = 0) in vec4 in_position;  
layout(location = 1) in vec4 in_color;  
layout(location = 2) in vec2 in_tex_cord;  
layout(location = 3) in float in_tex_id;  
  
uniform mat4 mvp;
out vec4 out_color;  
out vec2 out_tex_cord;  
out float out_tex_id;  
void main()  
{  
    out_color = in_color;  
    out_tex_cord = in_tex_cord;  
    out_tex_id = in_tex_id;  
    gl_Position = mvp * in_position;  
};
