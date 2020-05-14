#version 400 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D texSlot;
uniform float alpha;

void main()
{
	color = texture(texSlot, texCoords);
	color.a = color.a * alpha;
}