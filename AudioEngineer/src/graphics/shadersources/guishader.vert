#version 400 core

layout (location = 0) in vec2 vertex;

out vec2 texCoords;

uniform vec2 position;
uniform vec2 scaling;
uniform vec2 screenSize;

void main()
{
	texCoords = vec2(vertex.x, 1.0 - vertex.y);
	float x = 2.0 * (vertex.x * scaling.x + position.x) / screenSize.x - 1.0;
	float y = 1.0 - 2.0 * (vertex.y * scaling.y + position.y) / screenSize.y;
	gl_Position = vec4(x, y, 0, 1);
}