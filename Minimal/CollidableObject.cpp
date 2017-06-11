#include "CollidableObject.h"

CollidableObject::CollidableObject()
{
}
void CollidableObject::collisionResponse()
{

}
void CollidableObject::update(float time)
{

}
bool CollidableObject::isCollide(CollidableObject other)
{
	return false;
}
void CollidableObject::drawModel(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	model->draw(shaderProgram, projection, modelView);
}
void CollidableObject::drawBoundingBox(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	
}
CollidableObject::~CollidableObject()
{

}
