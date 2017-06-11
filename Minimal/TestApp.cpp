#include "TestApp.h"
#include <irrKlang.h>
#include <iostream>


using namespace std;
using namespace irrklang;

#define TEST_SCENE_VERTEX_SHADER_PATH "./testScene_shader.vert"
#define TEST_SCENE_FRAGMENT_SHADER_PATH "./testScene_shader.frag"

#define TEST_MODEL_VERTEX_SHADER_PATH "./testModel_shader.vert"
#define TEST_MODEL_FRAGMENT_SHADER_PATH "./testModel_shader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "./skyBox_shader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "./skyBox_shader.frag"

#define TOILET_MODEL_PATH "../ModelAssets/toilet/toilet.obj"

Cube * cube1;
Cube * cube2;
Model * toilet;

/*Flags for button presses*/

ISoundEngine * testSoundEngine = createIrrKlangDevice();
//rpc::client c1("localhost", 8080);

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
	skyBoxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);

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
	
	/* For the skybox */
	backGroundFaces.resize(6);
	backGroundFaces[0] = "SkyBoxTextures/BackGroundSkyBox/iceflow_rt.ppm";
	backGroundFaces[1] = "SkyBoxTextures/BackGroundSkyBox/iceflow_lf.ppm";
	backGroundFaces[2] = "SkyBoxTextures/BackGroundSkyBox/iceflow_up.ppm";
	backGroundFaces[3] = "SkyBoxTextures/BackGroundSkyBox/iceflow_dn.ppm";
	backGroundFaces[4] = "SkyBoxTextures/BackGroundSkyBox/iceflow_bk.ppm";
	backGroundFaces[5] = "SkyBoxTextures/BackGroundSkyBox/iceflow_ft.ppm";
	backGround = new SkyBox(backGroundFaces);

	toilet = new Model("../ModelAssets/toilet/toilet.obj");
	glm::mat4 toiletToWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));
	toilet->setToWorld(toiletToWorld);

	/* Initialize the forward direction */
	forward = glm::vec3(0.0f, 0.0f, -1.0f);

	xPressed = false;
	/* Play the music */
	testSoundEngine->play2D("Music/arrival_to_earth.mp3", GL_TRUE);

}
void TestApp::shutdownGl()
{

}
void TestApp::update()
{
	updateControllersAction();
	//c1.call("setModel1", CMat4(cube1->toWorld));
	//cube1->update();
	//cube2->toWorld = c1.call("getModel2").as<CMat4>().toGlm();
	//cube2->update();
	//pencil->spin(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}
void TestApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	glUseProgram(testSceneShaderProgram);
	cube1->draw(testSceneShaderProgram, projection, glm::inverse(headPose));
	cube2->draw(testSceneShaderProgram, projection, glm::inverse(headPose));

	glUseProgram(skyBoxShaderProgram);
	backGround->draw(projection, glm::inverse(headPose), skyBoxShaderProgram);

	glUseProgram(testModelShaderProgram);
	toilet->draw(testModelShaderProgram, projection, glm::inverse(headPose));
}
void TestApp::updateControllersAction()
{

	//get ovr exact system time
	double displayMidpointSeconds = ovr_GetPredictedDisplayTime(_session, 0);

	//get current ovrState, contains info such as head position, hand positions
	ovrTrackingState ts = ovr_GetTrackingState(_session, displayMidpointSeconds, ovrTrue);
	ovrPosef newOrigin = ts.HeadPose.ThePose;

	//cout << " In here at least " << endl;
	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState)))
	{
		/*
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
		*/
		
		glm::mat4 trans;

		bool leftPress = true;
	
		if (inputState.Thumbstick[ovrHand_Left].y < -0.500f)
		{
			trans = glm::translate(glm::mat4(1.0f), (-1.0f * forward) * 0.05f);
		}
		else if (inputState.Thumbstick[ovrHand_Left].y > 0.500f)
		{
			trans = glm::translate(glm::mat4(1.0f), forward * 0.05f);
		}
		else {
			leftPress = false;
		}
		if (inputState.Buttons == ovrTouch_X)
		{
			if (!xPressed)
			{
				xPressed = true;
				float oldY = forward.y;
				forward = glm::normalize(ovr::toGlm(rendEyePoses[0].Orientation) * glm::vec4(forward, 1.0f));
				forward.y = oldY;
			}
			else
			{
				xPressed = false;
			}
		}		
		else
		{
			xPressed = false;
		}
		if (leftPress) {
			glm::vec3 leftPos = glm::vec3(trans * glm::vec4(ovr::toGlm(rendEyePoses[0].Position), 1.0f));
			glm::vec3 rightPos = glm::vec3(trans * glm::vec4(ovr::toGlm(rendEyePoses[1].Position), 1.0f));
			rendEyePoses[0].Position = ovr::fromGlm(leftPos);
			rendEyePoses[1].Position = ovr::fromGlm(rightPos);
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
