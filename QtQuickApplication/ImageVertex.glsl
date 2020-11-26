#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec2 outTexCoord;

void main()
{
	outTexCoord = aTexCoord;
	gl_Position = vec4(aPos, 1.0);
}