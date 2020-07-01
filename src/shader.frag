#version 410 core
out vec4 FragColor;

in vec3 v3_color;

void main()
{
    FragColor = vec4(v3_color, 1.0f);
}