#version 460 core

in vec3 vertexNormal;
in vec3 FragPos;
in vec2 textureCoordinates;

uniform vec3 viewDirection;

out vec4 FragColor;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

const float offset = 1.0 / 600.0;

const vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
);

const float sobelFilterx[9] = {
	1, 0, -1,
	2, 0, -2,
	1, 0, -1
};

const float sobelFiltery[9] = {
    1, 2, 1,
    0, 0, 0,
    -1, -2, -1
};

vec3 applyFilter(const float[9] filt, vec3[9] texture)
{
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += texture[i] * filt[i];

    return col;
}

const float zNear = 0.1;
const float zFar = 100.0;

float linearizeDepth(float d)
{
    float z_n = 2.0 * d - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z_n * (zFar - zNear));
}

void main()
{
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(linearizeDepth(texture(depthTexture, textureCoordinates + offsets[i]).r));
    }

	vec3 colx = applyFilter(sobelFilterx, sampleTex);
    vec3 coly = applyFilter(sobelFiltery, sampleTex);

    vec3 col = abs(colx) + abs(coly);
    vec3 color = vec3(col);

	FragColor = texture(colorTexture, textureCoordinates) - vec4(color, 0.0);
}