#pragma once
#include "RiftApp.h"
#include "Shaders.h"
#include "Cube.h"
#include "vector"

class TestApp : public RiftApp
{
public:
	TestApp();
	~TestApp();
	void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) override;
	void updateControllersAction();
protected:
	void initGl() override;
	void shutdownGl() override;
	void update() override;
private:
	bool loadedShaders;
	GLint testSceneShaderProgram;	
};

