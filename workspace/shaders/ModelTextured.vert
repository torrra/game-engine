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

void main()
{
	vec4 pos4 = vec4(aPosition, 1.0);
	vec4 norm4 = vec4(aNormal, 1.0);
	vec4 position = pos4;
	vec4 resultNormal = norm4;

	if (isRigged)
	{
		for (uint index = 0; index < 4; ++index)
		{
			if (aBoneWeights[index] <= 0.0)
				continue;	

			vec4 transformation = pos4 * (skinningMatrices[aBoneIndices[index]] * aBoneWeights[index]);
			position += transformation;
			resultNormal += (norm4 * skinningMatrices[aBoneIndices[index]]) * aBoneWeights[index];

		}

		vertColor = vec3(0.0, 0.0, 1.0);
	}
	else
		vertColor = vec3(0.0, 1.0, 0.5);

	TexCoord = aTexCoord;
	Normal = normalMat * vec3(resultNormal);
	FragPos = vec3(model * position);

	mat4 viewModel = view * model;
	gl_Position =  projection * (viewModel * vec4(position.xyz, 1.0));
}
