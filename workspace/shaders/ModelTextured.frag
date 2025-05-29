#version 450 core

// Type definitions

struct OnmidirectionalLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuation;
};

struct DirectionalLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuation; 
};


struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuation;

    float innerCutoff;
    float outerCutoff;
};


const int DIFFUSE_MAP_BIT = 1;
const int NORMAL_MAP_BIT = 1 << 1;
const int SPECULAR_MAP_BIT = 1 << 2;
const int ROUGNESS_MAP_BIT = 1 << 3;
const int AMBIENT_OCCLUSION_MAP_BIT = 1 << 4;

// Uniform and buffer layouts

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

    int useMapsBits;
};


layout (std430, binding = 1) readonly buffer OmniLightBuf
{
    uint omniCount;
    OnmidirectionalLight omniLights[];
};

layout (std430, binding = 2) readonly buffer DirLightBuf
{
    uint dirCount;
    DirectionalLight dirLights[];
};

layout (std430, binding = 3) readonly buffer SpotLightBuf
{
    uint spotCount;
    SpotLight spotLights[];
};


uniform vec3 viewPos;


// in / out values

in vec4 VertColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

vec3 ApplyOmniLights(vec3 normal, vec3 viewDir);
vec3 ApplyDirLights(vec3 normal, vec3 viewDir);
vec3 ApplySpotLights(vec3 normal, vec3 viewDir);



// Global variables
vec3 g_objectDiffuse;



void main()
{
    vec4 diffuse4;


    if (bool(useMapsBits & DIFFUSE_MAP_BIT))
    {
        diffuse4 = texture(diffuseTexture, TexCoord);
        g_objectDiffuse = diffuse4.rgb;
    }
    else
    {
        g_objectDiffuse = diffuse;
        diffuse4 = vec4(diffuse, opacity);
    }


    vec3 normalizedNormal = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    if (0 == omniCount && 0 == dirCount && 0 == spotCount)
        FragColor = vec4(0.0, 1.0, 0.5, 1.0);

    else
    {
        FragColor.rgb = ApplyOmniLights(normalizedNormal, viewDirection) +
                        ApplyDirLights(normalizedNormal, viewDirection) +
                        ApplySpotLights(normalizedNormal, viewDirection);

        FragColor.a = diffuse4.a;
   }
} 


vec3 ApplyOmniLights(vec3 normal, vec3 viewDir)
{
    vec3 resultColor;           

    for (uint lightNum = 0; lightNum < omniCount; ++lightNum)
    {
        vec3 ambientResult = omniLights[lightNum].ambient * ambient;
        vec3 lightDirection = normalize(omniLights[lightNum].position - FragPos);
        float cosTheta = max(dot(normal, lightDirection), 0.f);
        vec3 diffuseResult = omniLights[lightNum].diffuse * (cosTheta * g_objectDiffuse);

        // Find halfway direction between light and eye direction for blinn-phong shading
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        float specularFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
        vec3 specularResult = omniLights[lightNum].specular * (specularFactor * specular);

        // Apply attenuation if applicable
        if (omniLights[lightNum].attenuation != vec3(0.f, 0.f, 0.f))
        {
             // Find fragment distance to light
            float dist = length(omniLights[lightNum].position - FragPos);

            float atten = omniLights[lightNum].attenuation.x;

            atten += omniLights[lightNum].attenuation.y * dist;
            atten += omniLights[lightNum].attenuation.z * (dist * dist);
            atten = 1.f / atten;

            ambientResult *= atten;
            diffuseResult *= atten;
            specularResult *= atten;

        }

        // Update fragment color (ambient + diffuse + specular)
        resultColor += ambientResult + diffuseResult + specularResult;
    }

    return resultColor;
}

vec3 ApplyDirLights(vec3 normal, vec3 viewDir)
{
    vec3 resultColor;

    for (uint lightNum = 0; lightNum < dirCount; ++lightNum)
    {
        vec3 ambientResult = dirLights[lightNum].ambient * ambient;
        vec3 lightDirection = normalize(-dirLights[lightNum].direction);
        float cosTheta = max(dot(normal, lightDirection), 0.f);
        vec3 diffuseResult = dirLights[lightNum].diffuse * (cosTheta * g_objectDiffuse);

        // Find halfway direction between light and eye direction for blinn-phong shading
        vec3 halfwayDir = normalize(lightDirection + viewDir);
        float specularFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
        vec3 specularResult = dirLights[lightNum].specular * (specularFactor * specular);

        // Update fragment color (ambient + diffuse + specular)
        resultColor += ambientResult + diffuseResult + specularResult;
    }

    return resultColor;
}

vec3 ApplySpotLights(vec3 normal, vec3 viewDir)
{
    vec3 resultColor;

    for (uint lightNum = 0; lightNum < spotCount; ++lightNum)
    {        
        // Find ambient light
        vec3 ambientResult = spotLights[lightNum].ambient * ambient;
        vec3 fragTolight = normalize(spotLights[lightNum].position - FragPos);
        vec3 lightDirection = normalize(-spotLights[lightNum].direction);

        // Get incidence angle cosine
        float cosTheta = max(dot(normal, lightDirection), 0.f);
        vec3 diffuseResult = spotLights[lightNum].diffuse * (cosTheta * g_objectDiffuse);
        float theta = dot(fragTolight, lightDirection);

        // Find difference beween cutoff cosines
        float epsilon   =  spotLights[lightNum].innerCutoff - spotLights[lightNum].outerCutoff;
        float intensity = clamp((theta - spotLights[lightNum].outerCutoff) / epsilon, 0.0f, 1.0f);
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        float specularFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
        vec3 specularResult = spotLights[lightNum].specular * (specularFactor * specular);

        // Decrease intensity if fragment is far fron the cone's center
        diffuseResult  *= intensity;
        specularResult *= intensity;

        // Apply attenuation if applicable
        if (spotLights[lightNum].attenuation != vec3(0.f, 0.f, 0.f))
        {
             // Find fragment distance to light
            float dist = length(spotLights[lightNum].position - FragPos);
            float atten = spotLights[lightNum].attenuation.x;

            atten += spotLights[lightNum].attenuation.y * dist;
            atten += spotLights[lightNum].attenuation.z * (dist * dist);
            atten = 1.f / atten;

            // Update color components
            ambientResult *= atten;
            diffuseResult *= atten;
            specularResult *= atten;

        }

       resultColor += resultColor += ambientResult + diffuseResult + specularResult;
    }

    return resultColor;
}