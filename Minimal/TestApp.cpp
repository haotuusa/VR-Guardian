#include "TestApp.h"

#define TEST_SCENE_VERTEX_SHADER_PATH "./testScene_shader.vert"
#define TEST_SCENE_FRAGMENT_SHADER_PATH "./testScene_shader.frag"

Cube * cube1;
Cube * cube2;

TestApp::TestApp()
{
	
}

void TestApp::initGl()
{
	RiftApp::initGl();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	ovr_RecenterTrackingOrigin(_session);

	testSceneShaderProgram = LoadShaders(TEST_SCENE_VERTEX_SHADER_PATH, TEST_SCENE_FRAGMENT_SHADER_PATH);

	/* Init the cubes */
	cube1 = new Cube(false);
	cube1->color = glm::vec3(1.0f, 0.0f, 0.0f);
	cube1->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	cube1->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)) * cube1->toWorld;
	cube1->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0.0f, 0.0f)) * cube1->toWorld;
	cube2 = new Cube(false);
	cube2->color = glm::vec3(0.0f, 0.0f, 1.0f);
	cube2->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	cube2->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)) * cube2->toWorld;
	cube2->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.0f)) * cube2->toWorld;
}
void TestApp::shutdownGl()
{

}
void TestApp::update()
{

}
void TestApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	glUseProgram(testSceneShaderProgram);
	cube1->draw(testSceneShaderProgram, projection, glm::inverse(headPose));
	cube2->draw(testSceneShaderProgram, projection, glm::inverse(headPose));
}
void TestApp::updateControllersAction()
{
	
}
TestApp::~TestApp()
{

}
