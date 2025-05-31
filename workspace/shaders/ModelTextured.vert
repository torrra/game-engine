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

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	vec4 position = vec4(aPosition, 1.0);

	if (isRigged)
	{
		float divisor = 1.0;
		float weightSum = aBoneWeights.x + aBoneWeights.y + aBoneWeights.z + aBoneWeights.w;

		if (weightSum > 0.0)
			divisor = 1.0 / weightSum;

		mat4 animMatrix = (divisor * aBoneWeights.w) * skinningMatrices[aBoneIndices.w] +
						  (divisor * aBoneWeights.z) * skinningMatrices[aBoneIndices.z] +
						  (divisor * aBoneWeights.y) * skinningMatrices[aBoneIndices.y] +
						  (divisor * aBoneWeights.x) * skinningMatrices[aBoneIndices.x];
						 
		position = animMatrix * position;
	}

	TexCoord = aTexCoord;
	Normal = normalMat * aNormal;
	FragPos = vec3(model * position);

	gl_Position = mvp * position;
}
