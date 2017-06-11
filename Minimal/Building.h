#pragma once
#include "CollidableObject.h"
class Building : public CollidableObject
{
public:
	Building(Model * model, glm::vec3 initPos, glm::vec3 initScale, Cube * cube);
	void drawBoundingBox(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	bool isCollide(CollidableObject other);
	void update(float time);
	void collisionResponse();
	~Building();
private:
	glm::vec3 groundPos;
	glm::vec3 initScale;
	glm::mat4 cubeInitToWorld;
};

