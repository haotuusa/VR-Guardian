#pragma once
#include "RiftApp.h"
#include "Shaders.h"
#include "Cube.h"
#include "vector"
#include <array>
#include <rpc/client.h>
#include "Model.h"

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
	GLint testModelShaderProgram;

};

