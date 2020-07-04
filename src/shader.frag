#version 410 core
out vec4 FragColor;
  
in vec3 v3_color;
in vec2 v2_tex_coord;

uniform sampler2D pp_texture;

void main()
{
    FragColor = texture(pp_texture, v2_tex_coord);
}