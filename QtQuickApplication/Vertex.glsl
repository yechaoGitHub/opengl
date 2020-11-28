#version 430 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 coordinate;
layout(location = 2) in vec3 velocity;
layout(location = 3) in vec3 acceleration;
layout(location = 4) in vec3 color;

out vec3 outCoordinate;
out vec3 outColor;

void main() 
{
	outCoordinate = coordinate;
	outColor = color;

	vec2 pos = (position.xy - 0.5) * 2.0;
	gl_Position = vec4(pos, 1.0, 1.0);
}

