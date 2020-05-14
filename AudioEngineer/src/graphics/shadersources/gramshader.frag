#version 400 core

in float value;

out vec4 color;

void main()
{
	color = vec4(value, 0, (1 - value), 1);
}