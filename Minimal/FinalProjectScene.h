#pragma once
#include "RiftApp.h"
#include "Shaders.h"
#include "Cube.h"
#include "vector"
#include "SkyBox.h"
#include <array>
#include <rpc/client.h>
#include "Model.h"
#include "CollidableObject.h"

struct CMat4 {
	std::array<float, 16> elements;
	MSGPACK_DEFINE_ARRAY(elements);

	CMat4() {}
	CMat4(glm::mat4 matrix) {
		memcpy(&elements[0], &matrix[0][0], 16 * sizeof(float));
	}

	glm::mat4 toGlm() {
		glm::mat4 matrix;
		memcpy(&matrix[0][0], &elements[0], 16 * sizeof(float));
		return matrix;
	}
};

class FinalProjectScene : public RiftApp
{
public:
	FinalProjectScene();
	~FinalProjectScene();

	void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) override;
	void renderCollidableObjects(const glm::mat4 & projection, const glm::mat4 & headPose);
	void updateControllersAction();
	void initGlassBuildings();
	void initWoodenBuildings();
	void initSkyScrapers();
	float getMidOff(float maxVal, float minVal);
	float getScale(float maxVal, float minVal);
protected:
	void initGl() override;	
	void shutdownGl() override;
	void update() override;
private:
	bool loadedShaders;
	GLint defSceneShaderProgram;
	GLint modelShaderProgram;
	GLint skyBoxShaderProgram;
	SkyBox * backGround;
	glm::vec3 forward;
	Model * ocean;
	Model * island;

	Model * glassBuilding1;
	Model * glassBuilding2;
	Model * glassBuilding3;
	Model * glassBuilding4;

	Model * woodBuilding1;
	Model * woodBuilding2;
	Model * woodBuilding3;
	Model * woodBuilding4;
	Model * woodBuilding5;

	Model * skyScraper1;
	Model * skyScraper2;
	Model * skyScraper3;
	Model * skyScraper4;
	Model * skyScraper5;

	Cube * sharedCube;

	vector< CollidableObject * > collidableObjects;
	vector <const GLchar *> backGroundFaces;
};


