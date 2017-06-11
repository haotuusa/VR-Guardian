#pragma once
#include "Model.h"
#include "Cube.h"

class CollidableObject
{
public:
	CollidableObject();
	bool isCollide(CollidableObject other);
	virtual void collisionResponse();
	virtual void update(float time);
	void drawModel(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	virtual void drawBoundingBox(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	~CollidableObject();
	Model * model;
	Cube * boundingCube;
	glm::mat4 cubeToWorld;
private:

};

