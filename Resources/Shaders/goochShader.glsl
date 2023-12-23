#version 460

in vec3 vertexNormal;
in vec3 FragPos;
in vec2 textureCoordinates;

uniform vec3 viewDirection; //Camera position

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Light light; 

const vec3 blueColor = vec3(0, 0, 1);
const vec3 yellowColor = vec3(1, 1, 0);

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

    vec3 warmColor = yellowColor + 0.6 * result;
    vec3 coolColor = blueColor + 0.2 * result;

    float interpolationFactor = (1 + dot(lightDir, norm)) / 2;
    vec3 finalColor = interpolationFactor * coolColor + (1 - interpolationFactor) * warmColor;


	FragColor = vec4(finalColor, 1.0);
}