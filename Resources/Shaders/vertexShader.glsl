#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in int index;

uniform mat4 viewProj;

layout(std430) buffer transforms
{
	mat4 data[];
};

out vec3 vertexNormal;
out vec3 FragPos;
out vec2 textureCoordinates;

void main()
{
	gl_Position = viewProj * data[index] * vec4(position, 1.0);

	vertexNormal = normal;
	FragPos = position;
	textureCoordinates = texCoords;
}