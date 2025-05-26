#version 450 core

layout (location = 0) in vec3 aPosition;

uniform mat4 mvp;
uniform vec4 aColor;

out vec4 fragColor;

void main()
{
	fragColor = aColor;
	gl_Position = mvp * vec4(aPosition, 1.0);
}