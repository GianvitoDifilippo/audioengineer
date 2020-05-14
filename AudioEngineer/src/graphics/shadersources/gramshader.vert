#version 400 core

layout (location = 0) in vec2 xy;
layout (location = 1) in float z;

out float value;

void main()
{
	gl_Position = vec4(xy, 0, 1);
	value = z;
}