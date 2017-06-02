#pragma once
#include "Line.h"
class Frustum
{

public:
	Frustum();
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	void setEndPoints(glm::vec3 eyePos, glm::vec3 corners[]);
	void setColor(glm::vec3 color);
	~Frustum();
private:
	Line * lines[4];

};

