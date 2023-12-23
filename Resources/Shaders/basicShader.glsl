#version 460 core

in vec3 vertexNormal;
in vec3 FragPos;
in vec2 textureCoordinates;

uniform vec3 viewDirection;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}