#version 450 core

// Type definitions

struct OnmidirectionalLight
{
    float positionArr[3];

    float ambientArr[3];
    float diffuseArr[3];
    float specularArr[3];

    float attenuationArr[3];
};

struct DirectionalLight
{
    float directionArr[3];
    
    float ambientArr[3];
    float diffuseArr[3];
    float specularArr[3];

    float attenuationArr[3]; 
};


struct SpotLight
{
    float positionArr[3];
    float directionArr[3];

    float ambientArr[3];
    float diffuseArr[3];
    float specularArr[3];

    float attenuationArr[3];

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

    // Can't use vec3 here because it messes up my precious, precious
    // buffer layout (vec3 aren't 12 bytes)

    float ambientArr[3];
    float diffuseArr[3];
    float specularArr[3];
    float emissiveArr[3];

    float shininess;
    float refractionIndex;
    float opacity;

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

// Material constants converted from their respective 32-bit float arrays
const vec3 ambient = vec3(ambientArr[0], ambientArr[1], ambientArr[2]);
const vec3 diffuse = vec3(diffuseArr[0], diffuseArr[1], diffuseArr[2]);
const vec3 specular = vec3(specularArr[0], specularArr[1], specularArr[2]);


// Color taken from texutre or material depnding
// on whether there is a texture or not
vec3 g_objectDiffuse;

vec3 ApplyOmniLights(vec3 normal, vec3 viewDir);
vec3 ApplyDirLights(vec3 normal, vec3 viewDir);
vec3 ApplySpotLights(vec3 normal, vec3 viewDir);


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
        g_objectDiffuse = vec3(diffuse);
        diffuse4 = vec4(diffuse, opacity);
    }


    vec3 normalizedNormal = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    if (0 == omniCount && 0 == dirCount && 0 == spotCount)
        FragColor = diffuse4;

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
        
        vec3 lightPosition = vec3(omniLights[lightNum].positionArr[0],
                                 omniLights[lightNum].positionArr[1],
                                 omniLights[lightNum].positionArr[2]);

        vec3 lightAmbient = vec3(omniLights[lightNum].ambientArr[0],
                                 omniLights[lightNum].ambientArr[1],
                                 omniLights[lightNum].ambientArr[2]);

        vec3 lightDiffuse = vec3(omniLights[lightNum].diffuseArr[0],
                                 omniLights[lightNum].diffuseArr[1],
                                 omniLights[lightNum].diffuseArr[2]);

        vec3 lightSpecular = vec3(omniLights[lightNum].specularArr[0],
                                  omniLights[lightNum].specularArr[1],
                                  omniLights[lightNum].specularArr[2]);

        vec3 ambientResult = lightAmbient * ambient;
        vec3 lightDirection = normalize(lightPosition - FragPos);
        float cosTheta = max(dot(normal, lightDirection), 0.f);
        vec3 diffuseResult = lightDiffuse * (cosTheta * g_objectDiffuse);

        // Find halfway direction between light and eye direction for blinn-phong shading
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        float specularFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
        vec3 specularResult = lightSpecular * (specularFactor * specular);

        // Apply attenuation if applicable
        if ((omniLights[lightNum].attenuationArr[0] != 0.f) ||
            (omniLights[lightNum].attenuationArr[1] != 0.f) ||
            (omniLights[lightNum].attenuationArr[2] != 0.f))
        {
             // Find fragment distance to light
            float dist = length(lightPosition - FragPos);

            float atten = omniLights[lightNum].attenuationArr[0];

            atten += omniLights[lightNum].attenuationArr[1] * dist;
            atten += omniLights[lightNum].attenuationArr[2] * (dist * dist);
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

        vec3 lightDirection = vec3(dirLights[lightNum].directionArr[0],
                                 dirLights[lightNum].directionArr[1],
                                 dirLights[lightNum].directionArr[2]);

        vec3 lightAmbient = vec3(dirLights[lightNum].ambientArr[0],
                                 dirLights[lightNum].ambientArr[1],
                                 dirLights[lightNum].ambientArr[2]);

        vec3 lightDiffuse = vec3(dirLights[lightNum].diffuseArr[0],
                                 dirLights[lightNum].diffuseArr[1],
                                 dirLights[lightNum].diffuseArr[2]);

        vec3 lightSpecular = vec3(dirLights[lightNum].specularArr[0],
                                  dirLights[lightNum].specularArr[1],
                                  dirLights[lightNum].specularArr[2]);

        vec3 ambientResult = lightAmbient * ambient;
        lightDirection = normalize(lightDirection);
        float cosTheta = max(dot(normal, lightDirection), 0.f);
        vec3 diffuseResult = lightDiffuse * (cosTheta * g_objectDiffuse);

        // Find halfway direction between light and eye direction for blinn-phong shading
        vec3 halfwayDir = normalize(lightDirection + viewDir);
        float specularFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
        vec3 specularResult = lightSpecular * (specularFactor * specular);

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

       vec3 lightPosition = vec3(spotLights[lightNum].positionArr[0],
                                 spotLights[lightNum].positionArr[1],
                                 spotLights[lightNum].positionArr[2]);

    
        vec3 lightDirection = vec3(spotLights[lightNum].directionArr[0],
                                   spotLights[lightNum].directionArr[1],
                                   spotLights[lightNum].directionArr[2]);

        vec3 lightAmbient = vec3(spotLights[lightNum].ambientArr[0],
                                 spotLights[lightNum].ambientArr[1],
                                 spotLights[lightNum].ambientArr[2]);

        vec3 lightDiffuse = vec3(spotLights[lightNum].diffuseArr[0],
                                 spotLights[lightNum].diffuseArr[1],
                                 spotLights[lightNum].diffuseArr[2]);

        vec3 lightSpecular = vec3(spotLights[lightNum].specularArr[0],
                                  spotLights[lightNum].specularArr[1],
                                  spotLights[lightNum].specularArr[2]);


        // Find ambient light
        vec3 ambientResult = lightAmbient * ambient;
        vec3 fragTolight = normalize(lightPosition - FragPos);
        lightDirection = normalize(lightDirection);

        // Get incidence angle cosine
        float cosTheta = max(dot(normal, lightDirection), 0.f);
        vec3 diffuseResult = lightDiffuse * (cosTheta * g_objectDiffuse);
        float theta = dot(fragTolight, lightDirection);

        // Find difference beween cutoff cosines
        float epsilon   =  spotLights[lightNum].outerCutoff - spotLights[lightNum].innerCutoff;
        float intensity = clamp((theta - spotLights[lightNum].outerCutoff) / epsilon, 0.0f, 1.0f);
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        float specularFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
        vec3 specularResult = lightSpecular * (specularFactor * specular);

        // Decrease intensity if fragment is far fron the cone's center
        diffuseResult  *= intensity;
        specularResult *= intensity;

        // Apply attenuation if applicable
        if ((spotLights[lightNum].attenuationArr[0] != 0.f) ||
            (spotLights[lightNum].attenuationArr[1] != 0.f) ||
            (spotLights[lightNum].attenuationArr[2] != 0.f))
        {
             // Find fragment distance to light
            float dist = length(lightPosition - FragPos);
            float atten = spotLights[lightNum].attenuationArr[0];

            atten += spotLights[lightNum].attenuationArr[1] * dist;
            atten += spotLights[lightNum].attenuationArr[2] * (dist * dist);
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