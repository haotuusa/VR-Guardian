#include "Quad.h"
#include <iostream>
using namespace std;

Quad::Quad()
{
	numVertices = 6;
	pa = glm::vec3(-1.0f, -1.0f, 0.0f);
	pb = glm::vec3(1.0f, -1.0f, 0.0f);
	pc = glm::vec3(-1.0f, 1.0f, 0.0f);
	pd = glm::vec3(1.0f, 1.0f, 0.0f);
	toWorld = glm::mat4(1.0f);
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords), quadTexCoords, GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		2, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		2 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
void Quad::setToWorld(glm::mat4 toWorld)
{	
	this->toWorld = toWorld;
	pa = toWorld * glm::vec4(pa, 1.0f);
	pb = toWorld * glm::vec4(pb, 1.0f);
	pc = toWorld * glm::vec4(pc, 1.0f);
	pd = toWorld * glm::vec4(pd, 1.0f);
}
glm::mat4 Quad::getProjection(glm::vec3 eyePos)
{
	glm::vec3 vr = glm::normalize(pb - pa);
	glm::vec3 vu = glm::normalize(pc - pa);
	glm::vec3 vn = glm::normalize(glm::cross(vr, vu));

	glm::vec3 va = pa - eyePos;
	glm::vec3 vb = pb - eyePos;
	glm::vec3 vc = pc - eyePos;

	float d = -1.0f * glm::dot(vn, va);
	float n = 0.1f;
	float f = 100.0f;

	float l = glm::dot(vr, va) * (n / d);
	float r = glm::dot(vr, vb) * (n / d);
	float b = glm::dot(vu, va) * (n / d);
	float t = glm::dot(vu, vc) * (n / d);

	glm::mat4 pTemp = glm::frustum(l, r, b, t, n, f);

	glm::mat4 m;
	m[0] = glm::vec4(vr, 0.0f);
	m[1] = glm::vec4(vu, 0.0f);
	m[2] = glm::vec4(vn, 0.0f);
	m[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 mT = glm::transpose(m);

	glm::mat4 T = glm::translate(glm::mat4(1.0f), -1.0f * eyePos);

	return pTemp * mT * T;
}
void Quad::getCorners(glm::vec3 & pa, glm::vec3 & pb, glm::vec3 & pc, glm::vec3 & pd)
{
	pa = this->pa;
	pb = this->pb;
	pc = this->pc;
	pd = this->pd;
}
void Quad::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	modelView = modelView * toWorld;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelView = glGetUniformLocation(shaderProgram, "modelview");
	//uColor = glGetUniformLocation(shaderProgram, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &(projection[0][0]));
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
	//glUniform3f(uColor, 1.0f, 0.0f, 0.0f);

	// Now to bind the texture 
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	/* figure out the width of the lines*/
	if (wired)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Tell OpenGL to draw with all the vertices 
	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
Quad::~Quad()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
