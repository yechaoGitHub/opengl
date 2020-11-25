#version 330

in vec3 outColor;
out vec3 fragColor;


uniform 

void main() 
{
	fragColor = vec4(outColor, 1.0f);
};

