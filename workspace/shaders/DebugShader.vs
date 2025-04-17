#version 450 core
layout (location = 0) in vec3 aPosition;

uniform mat4 projViewMatrix;
uniform vec4 aColor;

out vec4 fragColor;

void main()
{
	fragColor = aColor;
	gl_Position = projViewMatrix * vec4(aPosition, 1.0);
}