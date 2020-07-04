#version 410 core
out vec4 FragColor;
  
in vec3 v3_color;
in vec2 v2_tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend_amount;


void main()
{
    FragColor = mix(texture(texture1, v2_tex_coord), texture(texture2, v2_tex_coord), blend_amount);
}