#version 460

in vec3 vertexNormal;
in vec3 FragPos;
in vec2 textureCoordinates;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
};
  
uniform Light light; 

out vec4 FragColor;

void main()
{
	//Ambient Light
    float ambientLightStrength = 0.2;
	vec3 ambient = light.ambient * ambientLightStrength;

	//Diffuse Light
	vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;

	vec3 result = ambient + diffuse;
	FragColor = vec4(result, 1.0);
}