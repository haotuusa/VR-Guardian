#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 texCoords;


// You can output many things. The first vec4 type output determines the color of the fragment
layout(location = 0) out vec4 color;

uniform samplerCube skyBox;

void main()
{
	color = texture(skyBox, texCoords);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}