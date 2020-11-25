#version 330

in vec3 outColor;

void main() 
{
	gl_FragColor = vec4(outColor.xy, 0.0, 1.0f);
};

