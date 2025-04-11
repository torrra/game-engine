#version 450 core

uniform sampler2D texture1;

layout (std430, binding = 0) readonly buffer Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;

    float refractionIndex;
    float opacity;
    float shininess;
};

in vec2 texCoord;

out vec4 fragColor;

void main()
{
    fragColor = texture(texture1, texCoord);
} 