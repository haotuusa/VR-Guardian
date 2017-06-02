#pragma once
#include "Geode.h"
class Quad : public Geode
{
public:
	Quad();
	~Quad();
	void setToWorld(glm::mat4 toWorld);
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	glm::vec3 pa, pb, pc, pd;
	glm::mat4 getProjection(glm::vec3 eyePos);
	void getCorners(glm::vec3 & pa, glm::vec3 & pb, glm::vec3 & pc, glm::vec3 & pd);
private:
	int numVertices;
};
const GLfloat quadVertices[6][3] = {
	{-1.0f, -1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f},
	{-1.0f,  1.0f, 0.0f},
	{-1.0f,  1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f},
	{1.0f,  1.0f, 0.0f}
};
const GLfloat quadTexCoords[6][2] = {
	{0.0f, 0.0f},//
	{1.0f, 0.0f},//
	{0.0f, 1.0f},//
	{0.0f, 1.0f},//
    {1.0f, 0.0f},//
	{1.0f, 1.0f}//
};
