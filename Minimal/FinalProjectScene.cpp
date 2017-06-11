#include "FinalProjectScene.h"
#include "Building.h"
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


/*Flags for button presses*/
bool xPressed = false;
ISoundEngine * soundEngine = createIrrKlangDevice();
//rpc::client c1("localhost", 8080);

FinalProjectScene::FinalProjectScene()
{

}

void FinalProjectScene::initGl()
{
	RiftApp::initGl();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	ovr_RecenterTrackingOrigin(_session);

	defSceneShaderProgram = LoadShaders(TEST_SCENE_VERTEX_SHADER_PATH, TEST_SCENE_FRAGMENT_SHADER_PATH);
	modelShaderProgram = LoadShaders(TEST_MODEL_VERTEX_SHADER_PATH, TEST_MODEL_FRAGMENT_SHADER_PATH);
	skyBoxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);

	/* Initialize the sharedCube */
	sharedCube = new Cube(true);
	sharedCube->color = glm::vec3(1.0f, 0.0f, 0.0f);

	/* Init the ocean*/
	ocean = new Model("../ModelAssets/static_models/ocean/ocean.obj");
	//ocean->setToWorld(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f)));
	island = new Model("../ModelAssets/static_models/island/island.obj");	
	//island->setToWorld(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f)));*/

	initGlassBuildings(); 
	initWoodenBuildings();
	initSkyScrapers();

	/* The skyscraper */
	/*skyScraper1 = new Model("../ModelAssets/scene_models/buildingsky1.obj");
	skyScraper2 = new Model("../ModelAssets/scene_models/buildingsky2.obj");
	skyScraper3 = new Model("../ModelAssets/scene_models/buildingsky3.obj");
	skyScraper4 = new Model("../ModelAssets/scene_models/buildingsky4.obj");
	skyScraper5 = new Model("../ModelAssets/scene_models/buildingsky5.obj");*/

	/* For the skybox */
	backGroundFaces.resize(6);
	backGroundFaces[0] = "SkyBoxTextures/BackGroundSkyBox/iceflow_rt.ppm";
	backGroundFaces[1] = "SkyBoxTextures/BackGroundSkyBox/iceflow_lf.ppm";
	backGroundFaces[2] = "SkyBoxTextures/BackGroundSkyBox/iceflow_up.ppm";
	backGroundFaces[3] = "SkyBoxTextures/BackGroundSkyBox/iceflow_dn.ppm";
	backGroundFaces[4] = "SkyBoxTextures/BackGroundSkyBox/iceflow_bk.ppm";
	backGroundFaces[5] = "SkyBoxTextures/BackGroundSkyBox/iceflow_ft.ppm";
	backGround = new SkyBox(backGroundFaces);

	/* Initialize the forward direction */
	forward = glm::vec3(0.0f, 0.0f, -1.0f);

	/* Play the music */
	soundEngine->play2D("Music/arrival_to_earth.mp3", GL_TRUE);

	/* THe GROUND LEVEL STARTS AT 5.90f REMEMBER THIS!!!! */
}
float FinalProjectScene::getScale(float maxVal, float minVal)
{
	return ((maxVal - minVal) / 2.0f);
}
float FinalProjectScene::getMidOff(float maxVal, float minVal)
{
	return ((maxVal - minVal) / 2.0f) + minVal;
}
void FinalProjectScene::initGlassBuildings()
{
	/* The glass buildings */
	float yScale = getScale(90.3557f, 5.64008f);
	float yOff = getMidOff(90.3557f, 5.64008f);

	float xOff = getMidOff(-83.0736f, -158.191f);
	float zOff = getMidOff(296.529f, 269.183f);
	float xScale = getScale(-83.0736f, -158.191f);
	float zScale = getScale(296.529f, 269.183f);
	glm::vec3 genScale = glm::vec3(xScale, yScale, zScale);
	glassBuilding1 = new Model("../ModelAssets/static_models/buildingglass/buildingglass1.obj");
	collidableObjects.push_back(new Building(glassBuilding1, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(73.7551f, 45.6048f);
	zOff = getMidOff(253.404f, 177.998f);
	xScale = getScale(73.7551f, 45.6048f);
	zScale = getScale(253.404f, 177.998f);
	genScale = glm::vec3(xScale, yScale, zScale);
	glassBuilding2 = new Model("../ModelAssets/static_models/buildingglass/buildingglass2.obj");
	collidableObjects.push_back(new Building(glassBuilding2, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(-83.0736f, -167.063f);
	zOff = getMidOff(-158.191f, -194.409f);
	xScale = getScale(-83.0736f, -167.063f);
	zScale = getScale(-158.191f, -194.409f);
	genScale = glm::vec3(xScale, yScale, zScale);
	glassBuilding3 = new Model("../ModelAssets/static_models/buildingglass/buildingglass3.obj");
	collidableObjects.push_back(new Building(glassBuilding3, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(102.36f, 27.2426f); 
	zOff = getMidOff(-230.091f, -257.437f);
	xScale = getScale(102.36f, 27.2426f);
	zScale = getScale(-230.091f, -257.437f);
	genScale = glm::vec3(xScale, yScale, zScale);
	glassBuilding4 = new Model("../ModelAssets/static_models/buildingglass/buildingglass4.obj");
	collidableObjects.push_back(new Building(glassBuilding4, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));
}
void FinalProjectScene::initWoodenBuildings()
{
	float yOff = getMidOff(90.8498f, 5.76504f);
	float yScale = getScale(90.8498f, 5.76504f);

	float xOff = getMidOff(199.125f, 122.655f);
	float zOff = getMidOff(272.822f, 214.539f);
	float xScale = getScale(199.125f, 122.655f);
	float zScale = getScale(272.822f, 214.539f);
	glm::vec3 genScale = glm::vec3(xScale, yScale, zScale);
	woodBuilding1 = new Model("../ModelAssets/static_models/buildingwood/buildingwood1.obj");
	collidableObjects.push_back(new Building(woodBuilding1, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(35.5335f, -40.9363f);
	zOff = getMidOff(10.4698f, -47.8133f);
	xScale = getScale(35.5335f, -40.9363f);
	zScale = getScale(10.4698f, -47.8133f);
	woodBuilding2 = new Model("../ModelAssets/static_models/buildingwood/buildingwood2.obj");
	collidableObjects.push_back(new Building(woodBuilding2, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(283.121f, 206.651f);
	zOff = getMidOff(-237.309f, -295.592f);
	xScale = getScale(283.121f, 206.651f);
	zScale = getScale(-237.309f, -295.592f);
	woodBuilding3 = new Model("../ModelAssets/static_models/buildingwood/buildingwood3.obj");
	collidableObjects.push_back(new Building(woodBuilding3, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(-142.623f, -219.092f);
	zOff = getMidOff(-91.2998f, -149.583f);
	xScale = getScale(-142.623f, -219.092f);
	zScale = getScale(-91.2998f, -149.583f);
	woodBuilding4 = new Model("../ModelAssets/static_models/buildingwood/buildingwood4.obj");
	collidableObjects.push_back(new Building(woodBuilding4, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(-229.392f, -305.862f);
	zOff = getMidOff(-237.309f, -295.592f);
	xScale = getScale(-229.392f, -305.862f);
	zScale = getScale(-237.309f, -295.592f);
	woodBuilding5 = new Model("../ModelAssets/static_models/buildingwood/buildingwood5.obj");
	collidableObjects.push_back(new Building(woodBuilding5, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));
}
void FinalProjectScene::initSkyScrapers()
{
	float yOff = getMidOff(169.585f, 5.59118f);
	float yScale = getScale(169.585f, 5.59118f);

	float xOff = getMidOff(-212.344f, -275.929f);
	float zOff = getMidOff(198.569f, 134.984f);
	float xScale = getScale(-212.344f, -275.929f);
	float zScale = getScale(198.569f, 134.984f);
	glm::vec3 genScale = glm::vec3(xScale, yScale, zScale);
	skyScraper1 = new Model("../ModelAssets/static_models/buildingsky/buildingsky1.obj");
	collidableObjects.push_back(new Building(skyScraper1, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(-55.6536f, -119.239f);
	zOff = getMidOff(206.29f, 142.705f);
	xScale = getScale(-55.6536f, -119.239f);
	zScale = getScale(206.29f, 142.705f);
	genScale = glm::vec3(xScale, yScale, zScale);
	skyScraper2 = new Model("../ModelAssets/static_models/buildingsky/buildingsky2.obj");
	collidableObjects.push_back(new Building(skyScraper2, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(-75.7724f, -139.358f);
	zOff = getMidOff(32.011f, -31.5742f);
	xScale = getScale(-75.7724f, -139.358f);
	zScale = getScale(32.011f, -31.5742f);
	genScale = glm::vec3(xScale, yScale, zScale);
	skyScraper3 = new Model("../ModelAssets/static_models/buildingsky/buildingsky3.obj");
	collidableObjects.push_back(new Building(skyScraper3, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(234.458f, 170.873f);
	zOff = getMidOff(154.8f, 91.2145f);
	xScale = getScale(234.458f, 170.873f);
	zScale = getScale(154.8f, 91.2145f);
	genScale = glm::vec3(xScale, yScale, zScale);
	skyScraper4 = new Model("../ModelAssets/static_models/buildingsky/buildingsky4.obj");
	collidableObjects.push_back(new Building(skyScraper4, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));

	xOff = getMidOff(234.458f, 170.873f);
	zOff = getMidOff(-79.4665f, -143.052f);
	xScale = getScale(234.458f, 170.873f);
	zScale = getScale(-79.4665f, -143.052f);
	genScale = glm::vec3(xScale, yScale, zScale);
	skyScraper5 = new Model("../ModelAssets/static_models/buildingsky/buildingsky5.obj");
	collidableObjects.push_back(new Building(skyScraper5, glm::vec3(xOff, yOff, zOff), genScale, sharedCube));
}
void FinalProjectScene::shutdownGl()
{

}
void FinalProjectScene::update()
{
	updateControllersAction();
	//c1.call("setModel1", CMat4(cube1->toWorld));
	//cube1->update();
	//cube2->toWorld = c1.call("getModel2").as<CMat4>().toGlm();
	//cube2->update();
	//pencil->spin(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}
void FinalProjectScene::renderCollidableObjects(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	for (int i = 0; i < collidableObjects.size(); i++)
	{
		glUseProgram(modelShaderProgram);
		collidableObjects[i]->drawModel(modelShaderProgram, projection, glm::inverse(headPose));

		glUseProgram(defSceneShaderProgram);
		collidableObjects[i]->drawBoundingBox(defSceneShaderProgram, projection, glm::inverse(headPose));
	}
}
void FinalProjectScene::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	glUseProgram(skyBoxShaderProgram);
	backGround->draw(projection, glm::inverse(headPose), skyBoxShaderProgram);

	renderCollidableObjects(projection, headPose);

	glUseProgram(modelShaderProgram);
	ocean->draw(modelShaderProgram, projection, glm::inverse(headPose));

	island->draw(modelShaderProgram, projection, glm::inverse(headPose));

	/*woodBuilding1->draw(modelShaderProgram, projection, glm::inverse(headPose));
	woodBuilding2->draw(modelShaderProgram, projection, glm::inverse(headPose));
	woodBuilding3->draw(modelShaderProgram, projection, glm::inverse(headPose));
	woodBuilding4->draw(modelShaderProgram, projection, glm::inverse(headPose));
	woodBuilding5->draw(modelShaderProgram, projection, glm::inverse(headPose));

	skyScraper1->draw(modelShaderProgram, projection, glm::inverse(headPose));*/
}
void FinalProjectScene::updateControllersAction()
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
FinalProjectScene::~FinalProjectScene()
{

}