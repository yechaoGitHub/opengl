#version 330

in vec3 outColor;
in vec3 outCoordinate;
out vec4 fragColor;

uniform sampler2D tex;

void main() 
{
	vec2 texCrood = vec2(outCoordinate.x, 1.0 - outCoordinate.y);
	vec4 color = texture(tex, texCrood);
	fragColor = vec4(color.bgr, 1.0f);
};

