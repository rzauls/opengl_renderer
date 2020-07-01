#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 v3_color;

uniform float xOffset;
uniform float f_time;

void main()
{
    gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);
    v3_color = vec3(aColor.x * f_time, aColor.y * f_time, aColor.z * f_time);
}