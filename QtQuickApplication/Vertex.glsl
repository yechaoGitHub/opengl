#version 430 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
layout(location = 2) in vec3 acceleration;
layout(location = 3) in vec3 color;

out vec3 outColor;

void main() 
{
	outColor = color;
	gl_Position = vec4(position.xy, 1.0, 1.0);
}

