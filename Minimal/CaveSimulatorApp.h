#pragma once
#include "RiftApp.h"
#include "ColorCubeScene.h"
#include "Shaders.h"
#include "Quad.h"
#include "vector"
#include "SkyBox.h"
#include "Frustum.h"

class CaveSimulatorApp: public RiftApp
{
	ColorCubeScene * cubeScene;
	GLint cubeSceneShaderProgram;
	GLint skyBoxShaderProgram;
	GLint vrShaderProgram;
	GLint frustumShaderProgram;
	GLuint frameBufferNames[3];
	GLuint renderedTextures[3];
	GLuint depthRenderBuffers[3];
	int numScreens;

	std::vector< const GLchar *> leftEyeFaces;
	std::vector< const GLchar *> rightEyeFaces;
	std::vector< const GLchar *> outerSkyFaces;

	SkyBox * leftEyeSkyBox;
	SkyBox * rightEyeSkyBox;
	SkyBox * outerMonoSkyBox;

	Frustum * frusts[3];
	glm::vec3 pas[3];
	glm::vec3 pbs[3];
	glm::vec3 pcs[3];
	glm::vec3 pds[3];

	glm::mat4 oldLeftEyePose;
	glm::mat4 oldRightEyePose;

	glm::mat4 newLeftEyePose;
	glm::mat4 newRightEyePose;

	Quad * leftWall;
	Quad * rightWall;
	Quad * floor;
	bool loadedShaders;
	vector< Quad * > screens;
public:
	CaveSimulatorApp();
	~CaveSimulatorApp();
	void renderToTexture(const glm::mat4 & projection, const glm::mat4 & headPose);
	bool initFrameBuffer();
	void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) override;
	void updateControllersAction();
	glm::mat4 getHeadPose(const glm::mat4 & headPose);
	void drawFrustums(const glm::mat4 & projection, const glm::mat4 caveHeadPose, const glm::mat4 & headPose);
protected:
	void initGl() override;
	void shutdownGl() override;
	void update() override;
};

