#version 450 core

layout(location = 0) in vec3 modelSpacePosition;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(modelSpacePosition, 1);
}