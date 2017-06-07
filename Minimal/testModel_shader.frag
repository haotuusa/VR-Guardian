#version 410 core

out vec4 fragColor;
in vec2 outTexCoord;

uniform sampler2D texture_diffuse1;

void main(void) {   
	fragColor = texture(texture_diffuse1, outTexCoord);
}