#version 460 core  
layout(location = 0) out vec4 color;  
 
in vec4  out_color;  
in vec2  out_tex_cord;  
in float out_tex_id;  
 
uniform sampler2D textures[32];  
 
void main()  
{  
    int index = int(out_tex_id);  
    color = texture(textures[index], out_tex_cord) * out_color; 
};
