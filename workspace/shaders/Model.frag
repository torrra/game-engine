#version 450 core

uniform vec3 objectColor;

out vec4 fragColor;
in vec3 vertColor;

void main()
{
    fragColor = vec4(vertColor, 1.0);
   // fragColor = vec4(objectColor, 1.0);
} 