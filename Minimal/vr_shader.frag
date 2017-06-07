#version 410 core

out vec4 fragColor;
in vec2 outTexCoord;

uniform sampler2D renderedTexture;

void main(void){
	fragColor = texture(renderedTexture, outTexCoord);
	//fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}