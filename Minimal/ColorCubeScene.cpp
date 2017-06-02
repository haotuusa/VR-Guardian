#include "ColorCubeScene.h"
#include <iostream>
#include <vector>

using namespace std;
ColorCubeScene::ColorCubeScene()
{
	cube = new Cube(false);
	cube->scaleVal = glm::vec3(1.0f, 1.0f, 1.0f);
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.4f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	cubeScaleVal = 0.1f;
	cube->setToWorld(toWorld);
}
void ColorCubeScene::render(const mat4 & projection, const mat4 & modelview, GLint shaderProgram)
{
	cube->draw(shaderProgram, projection, modelview);
}
void ColorCubeScene::loadTextures(const char * fileName)
{
	cube->loadTextures(fileName);
}
void ColorCubeScene::expandCube()
{
	cout << " Expanding cube " << endl;
	if (cubeScaleVal < 100.0f)
	{
		cubeScaleVal *= 1.002f;
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.002f));
	}
	cout << " The cube scale val " << cubeScaleVal << endl;
}
void ColorCubeScene::contractCube()
{
	cout << "Contracting cube " << endl;
	if (cubeScaleVal > 0.01f)
	{
		cubeScaleVal *= 0.998f;
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.998f));
	}
	cout << " The cube scale val " << cubeScaleVal << endl;
}
void ColorCubeScene::resetCubeSize()
{
	cout << " Resetting cube " << endl;
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f / cubeScaleVal));
	cubeScaleVal = 0.1f;
	cout << " The cube scale val " << cubeScaleVal << endl;
}
void ColorCubeScene::translateCube(glm::vec3 transDir)
{
	toWorld = glm::translate(glm::mat4(1.0f), transDir) * toWorld;
}
void ColorCubeScene::update()
{
	cube->setToWorld(toWorld);
	//cube->spin(1.0f);
//	cout << " In here updating " << endl;
}
ColorCubeScene::~ColorCubeScene()
{

}
