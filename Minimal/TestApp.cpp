#include "TestApp.h"
#include <iostream>

using namespace std;

#define TEST_SCENE_VERTEX_SHADER_PATH "./testScene_shader.vert"
#define TEST_SCENE_FRAGMENT_SHADER_PATH "./testScene_shader.frag"

#define TEST_MODEL_VERTEX_SHADER_PATH "./testModel_shader.vert"
#define TEST_MODEL_FRAGMENT_SHADER_PATH "./testModel_shader.frag"

Cube * cube1;
Cube * cube2;
Model * texCube;

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
	testModelShaderProgram = LoadShaders(TEST_MODEL_VERTEX_SHADER_PATH, TEST_MODEL_FRAGMENT_SHADER_PATH);

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

	string pencilDir = "../ModelAssets/cube/";
	texCube = new Model("../ModelAssets/cube/cube.obj", pencilDir);
	glm::mat4 penToWorld = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	//penToWorld = penToWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
	texCube->setToWorld(penToWorld);

}
void TestApp::shutdownGl()
{

}
void TestApp::update()
{
	updateControllersAction();
	//rpc::client c1("localhost", 8080);
	//c1.call("setModel1", CMat4(cube1->toWorld));
	cube1->update();
	//cube2->toWorld = c1.call("getModel2").as<CMat4>().toGlm();
	cube2->update();
	//pencil->spin(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}
void TestApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	glUseProgram(testSceneShaderProgram);
	cube1->draw(testSceneShaderProgram, projection, glm::inverse(headPose));
	cube2->draw(testSceneShaderProgram, projection, glm::inverse(headPose));

	glUseProgram(testModelShaderProgram);
	texCube->draw(testModelShaderProgram, projection, glm::inverse(headPose));
}
void TestApp::updateControllersAction()
{
	//cout << " In here at least " << endl;
	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState)))
	{
		if (inputState.Thumbstick[ovrHand_Right].x < -0.500f)
		{
			//cout << " moving left " << endl;
			cube1->moveCube(glm::vec3(-0.01f, 0.0f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Right].x > 0.500f)
		{
			cube1->moveCube(glm::vec3(0.01f, 0.0f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Right].y < -0.500f)
		{
			cube1->moveCube(glm::vec3(0.0f, -0.01f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Right].y > 0.500f)
		{
			cube1->moveCube(glm::vec3(0.0f, 0.01f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Left].y < -0.500f)
		{
			cube1->moveCube(glm::vec3(0.0f, 0.0f, 0.01f));
		}
		if (inputState.Thumbstick[ovrHand_Left].y > 0.500f)
		{
			cube1->moveCube(glm::vec3(0.0f, 0.0f, -0.01f));
		}
	}
	else
	{
		cout << " The session has failed " << endl;
	}
}
TestApp::~TestApp()
{

}
