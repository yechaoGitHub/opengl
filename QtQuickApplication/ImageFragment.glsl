#version 430 core

in vec2 outTexCoord;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    vec2 texCrood = vec2(outTexCoord.x, 10 - outTexCoord.y);
    vec4 color = texture(tex, texCrood);
    FragColor = vec4( color.bgr, 1.0);
}

