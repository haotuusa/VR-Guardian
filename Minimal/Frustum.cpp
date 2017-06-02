#include "Frustum.h"

Frustum::Frustum()
{
	for (int i = 0; i < 4; i++)
	{
		lines[i] = new Line(glm::vec3(0.0f));
	}
}
void Frustum::setColor(glm::vec3 color)
{
	for (int i = 0; i < 4; i++)
	{
		lines[i]->setColor(color);
	}
}
void Frustum::setEndPoints(glm::vec3 eyePos, glm::vec3 corners[])
{
	for (int i = 0; i < 4; i++)
	{
		lines[i]->setEndPoints(eyePos, corners[i]);
		lines[i]->loadLineVertices();
	}
}
void Frustum::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	for (int i = 0; i < 4; i++)
	{
		lines[i]->draw(shaderProgram, projection, modelView);
	}
}
Frustum::~Frustum()
{
}
