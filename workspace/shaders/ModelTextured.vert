#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 4) in vec2 aTexCoord;
layout (location = 5) in vec4 aVertexColor;
layout (location = 6) in uvec4 aBoneIndices;
layout (location = 7) in vec4 aBoneWeights;

uniform mat4 model;
uniform mat4 mvp;
uniform mat3 normalMat;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	TexCoord = aTexCoord;
	Normal = normalMat * aNormal;
	FragPos = vec3(model * vec4(aPosition, 1.0));

	gl_Position = mvp * vec4(aPosition, 1.0);
}
