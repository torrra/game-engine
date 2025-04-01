#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 fragColor;

uniform mat4 projViewMatrix;

void main()
{
	fragColor = vec3(0.0, 1.0, 0.0);
	gl_Position = projViewMatrix * vec4(aPosition, 1.0);
}