#include "Building.h"



Building::Building(Model * model, glm::vec3 initPos, glm::vec3 initScale, Cube * cube)
{
	this->model = model;
	this->groundPos = initPos;
	this->boundingCube = cube;
	cubeInitToWorld = glm::translate(glm::mat4(1.0f), initPos);
	cubeInitToWorld = cubeInitToWorld * glm::scale(glm::mat4(1.0f), initScale);
	cube->setToWorld(cubeInitToWorld);
}
void Building::drawBoundingBox(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	boundingCube->setToWorld(cubeInitToWorld * model->getToWorld());
	boundingCube->draw(shaderProgram, projection, modelView);
}
void Building::collisionResponse()
{
	//Do nothing because a building cannot move
	//Maybe get destroyed or something 
}
void Building::update(float time)
{

}
bool Building::isCollide(CollidableObject other)
{
	return false;
}
Building::~Building()
{
}
