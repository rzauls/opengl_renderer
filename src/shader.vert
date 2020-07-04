#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 v3_color;
out vec2 v2_tex_coord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    v3_color = aColor;
    v2_tex_coord = aTexCoord;
}