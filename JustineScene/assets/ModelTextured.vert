#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 4) in vec2 aTexCoord;
layout (location = 5) in vec4 aVertexColor;
layout (location = 6) in ivec4 aBoneIndices;
layout (location = 7) in vec4 aBoneWeights;


layout (std430, binding = 4) readonly buffer Skinning
{
    mat4 skinningMatrices[];
};

uniform bool isRigged;
uniform mat4 model;
uniform mat4 mvp;
uniform mat3 normalMat;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 vertColor;

void AnimSkinning(inout vec3 inPos, inout vec3 inNormal, vec4 inWeights, vec4 inIndices)
{
    vec4 outPosition = vec4(0.0);
    vec3 outNormal = vec3(0.0);

    float totalWeight = 1.0 / (inWeights[0] + inWeights[1] + inWeights[2] + inWeights[3]);
    vec4 weights = {
        inWeights[0] * totalWeight,
        inWeights[1] * totalWeight,
        inWeights[2] * totalWeight,
        inWeights[3] * totalWeight
    };

    for (int i = 0; i < 4; ++i) 
    {
        if (weights[i] > 0.0f) 
        {
            outPosition += vec4(inPos, 1.0) * skinningMatrices[int(inIndices[i])] * weights[i];
            
            mat4 boneMatrix = skinningMatrices[int(inIndices[i])];
            outNormal += weights[i] * mat3(boneMatrix) * inNormal;
        }
    }

    inPos = outPosition.xyz;
    inNormal = outNormal;
}

void main()
{
    vec3 pos = aPosition;
    vec3 norm = aNormal;

    if (isRigged)
    {
        AnimSkinning(pos, norm, aBoneWeights, aBoneIndices);
        vertColor = vec3(0.0, 0.0, 1.0);
    }
    else
        vertColor = vec3(0.0, 1.0, 0.5);

    TexCoord = aTexCoord;
    Normal = normalize(normalMat * vec3(norm));
    FragPos = vec3(model * vec4(pos, 1.0));

    gl_Position =  mvp * vec4(pos.xyz, 1.0);
}