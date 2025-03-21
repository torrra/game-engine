#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 4) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 mvp;
uniform mat3 normalMat;

out vec3 fragPos;

void main()
{
	fragPos = vec4(model * vec4(aPosition, 1.0)).xyz;

	gl_Position = mvp * vec4(aPosition, 1.0);
}
