#version 450 core

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 2) uniform sampler2D specularMap;
layout(binding = 3) uniform sampler2D diffuseRoughness;
layout(binding = 4) uniform sampler2D ambientOcclusionMap;

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

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    FragColor = texture(diffuseTexture, TexCoord);
} 