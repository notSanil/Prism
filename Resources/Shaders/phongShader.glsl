#version 460

in vec3 vertexNormal;
in vec3 FragPos;
in vec2 textureCoordinates;

uniform vec3 viewDirection;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;
uniform Light light; 

out vec4 FragColor;

void main()
{
	//Ambient Light
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoordinates));

	//Diffuse Light
	vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoordinates));  

	//Specular Light
	vec3 viewDir = normalize(viewDirection - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoordinates));


	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}