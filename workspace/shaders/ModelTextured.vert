#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 4) in vec2 aTexCoord;
layout (location = 5) in vec4 aVertexColor;
layout (location = 6) in uvec4 aBoneIndices;
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

void main()
{
	vec4 pos4 = vec4(aPosition, 1.0);
	vec4 position = vec4(aPosition, 1.0);

	if (isRigged)
	{
		float divisor = 1.0;
		float weightSum = aBoneWeights.x + aBoneWeights.y + aBoneWeights.z + aBoneWeights.w;

		if (weightSum > 0.0)
			divisor = 1.0 / weightSum;

		//mat4 animMatrix = (divisor * aBoneWeights.w) * skinningMatrices[aBoneIndices.w] +
		//				  (divisor * aBoneWeights.z) * skinningMatrices[aBoneIndices.z] +
		//				  (divisor * aBoneWeights.y) * skinningMatrices[aBoneIndices.y] +
		//				  (divisor * aBoneWeights.x) * skinningMatrices[aBoneIndices.x];
		//



	/*resultPos += (pos * skin.mat[int(boneIndices[0])]) * boneWeights[0];
	resultPos += (pos * skin.mat[int(boneIndices[1])]) * boneWeights[1];
	resultPos += (pos * skin.mat[int(boneIndices[2])]) * boneWeights[2];
	resultPos += (pos * skin.mat[int(boneIndices[3])]) * boneWeights[3];*/
		
	for (uint index = 0; index < 4; ++index)
	{
		if (aBoneWeights[index] < 0.0)
	    {
			vertColor = vec3(1.0, 0.0, 0.0);
			continue;
		}
	
		position += (pos4 * skinningMatrices[aBoneIndices[index]]) * aBoneWeights[index];
		vertColor = vec3(0.0, 0.0, 1.0);
	
	}

		//position += (pos4 * skinningMatrices[aBoneIndices[0]]) * aBoneWeights[0];
		//position += (pos4 * skinningMatrices[aBoneIndices[1]]) * aBoneWeights[1];
		//position += (pos4 * skinningMatrices[aBoneIndices[2]]) * aBoneWeights[2];
		//position += (pos4 * skinningMatrices[aBoneIndices[3]]) * aBoneWeights[3];
	}
	else
		vertColor = vec3(0.0, 1.0, 0.5);

	TexCoord = aTexCoord;
	Normal = normalMat * aNormal;
	FragPos = vec3(model * position);

	mat4 viewModel = view * model;
	gl_Position =  projection * (viewModel * position);
}
