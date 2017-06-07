#version 410 core

layout(location = 0) out vec4 fragColor;
in vec2 outTexCoord;

void main(void) {   
    fragColor = texture(ourTexture, outTexCoord);
}