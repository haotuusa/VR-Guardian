#include "CaveSimulatorApp.h"
#include <iostream>
#define CUBE_SCENE_VERTEX_SHADER_PATH "./cubeScene_shader.vert"
#define CUBE_SCENE_FRAGMENT_SHADER_PATH "./cubeScene_shader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "./skyBox_shader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "./skyBox_shader.frag"

#define VR_VERTEX_SHADER_PATH "./vr_shader.vert"
#define VR_FRAGMENT_SHADER_PATH "./vr_shader.frag"

#define VR_FRUSTUM_VERTEX_SHADER_PATH "./frustum_shader.vert"
#define VR_FRUSTUM_FRAGMENT_SHADER_PATH "./frustum_shader.frag"

float PI = 3.1415927;

bool aPressed = false;
bool yPressed = false;
bool bPressed = false;
bool freeze = false;;
bool triggerPressed = false;

CaveSimulatorApp::CaveSimulatorApp()
{
	loadedShaders = false;

	/* Load the left eye faces */
	leftEyeFaces.resize(6);
	leftEyeFaces[0] = "SkyBoxTextures/left-ppm/px.ppm";
	leftEyeFaces[1] = "SkyBoxTextures/left-ppm/nx.ppm";
	leftEyeFaces[2] = "SkyBoxTextures/left-ppm/py.ppm";
	leftEyeFaces[3] = "SkyBoxTextures/left-ppm/ny.ppm";
	leftEyeFaces[4] = "SkyBoxTextures/left-ppm/pz.ppm";
	leftEyeFaces[5] = "SkyBoxTextures/left-ppm/nz.ppm";

	/* Load the right eye faces */
	rightEyeFaces.resize(6);
	rightEyeFaces[0] = "SkyBoxTextures/right-ppm/px.ppm";
	rightEyeFaces[1] = "SkyBoxTextures/right-ppm/nx.ppm";
	rightEyeFaces[2] = "SkyBoxTextures/right-ppm/py.ppm";
	rightEyeFaces[3] = "SkyBoxTextures/right-ppm/ny.ppm";
	rightEyeFaces[4] = "SkyBoxTextures/right-ppm/pz.ppm";
	rightEyeFaces[5] = "SkyBoxTextures/right-ppm/nz.ppm";

	/* Load the outer sky faces*/
	outerSkyFaces.resize(6);
	outerSkyFaces[0] = "SkyboxTextures/outerSkyBox/interstellar_rt.ppm";
	outerSkyFaces[1] = "SkyboxTextures/outerSkyBox/interstellar_lf.ppm";
	outerSkyFaces[2] = "SkyboxTextures/outerSkyBox/interstellar_up.ppm";
	outerSkyFaces[3] = "SkyboxTextures/outerSkyBox/interstellar_dn.ppm";
	outerSkyFaces[4] = "SkyboxTextures/outerSkyBox/interstellar_bk.ppm";
	outerSkyFaces[5] = "SkyboxTextures/outerSkyBox/interstellar_ft.ppm";
}
void CaveSimulatorApp::initGl() {
	RiftApp::initGl();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	ovr_RecenterTrackingOrigin(_session);

	/* Initialize the scene */
	cubeScene = new ColorCubeScene();
	leftWall = new Quad();
	rightWall = new Quad();
	floor = new Quad();

	float zOffset = 2.0f;

	/* Position the floor */
	glm::mat4 floorToWorld = glm::rotate(glm::mat4(1.0f), 45.0f * (PI / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	float rotAng = 90.0f * (PI / 180.0f);
	floorToWorld = glm::rotate(rotAng, glm::vec3(-1.0f, 0.0f, 0.0f)) * floorToWorld;
	floorToWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -5.0f + zOffset)) * floorToWorld;	
	floorToWorld = floorToWorld * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
	floor->setToWorld(floorToWorld);
	floor->wired = false;

	/* Position the left wall */
	glm::mat4 leftWallToWorld = glm::rotate(glm::mat4(1.0f), 45.0f * (PI / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	leftWallToWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5355f, 0.0f, -8.5355f + zOffset)) * leftWallToWorld;
	leftWallToWorld = leftWallToWorld * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
	leftWall->setToWorld(leftWallToWorld);
	leftWall->wired = false;

	/* Position of the right wall */
	glm::mat4 rightWallToWorld = glm::rotate(glm::mat4(1.0f), 45.0f * (PI / 180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	rightWallToWorld = glm::translate(glm::mat4(1.0f), glm::vec3(3.5355f, 0.0f, -8.5355f + zOffset)) * rightWallToWorld;
	rightWallToWorld = rightWallToWorld * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
	rightWall->setToWorld(rightWallToWorld);
	rightWall->wired = false;

	/* Store the screens */
	screens.push_back(leftWall);
	screens.push_back(rightWall);
	screens.push_back(floor);

	numScreens = 3;

	for (int i = 0; i < numScreens; i++)
	{
		frusts[i] = new Frustum();
	}

	/* Load the textures */
	cubeScene->loadTextures("vr_test_pattern.ppm");	

	/* Init the skyboxes */
	leftEyeSkyBox = new SkyBox(leftEyeFaces);
	rightEyeSkyBox = new SkyBox(rightEyeFaces);

	/* Init the outer skybox */
	outerMonoSkyBox = new SkyBox(outerSkyFaces);

	/* init the frame buffer */
	cout << " The frame buffer status " << initFrameBuffer() << endl;;
}
bool CaveSimulatorApp::initFrameBuffer()
{
	bool success = true;
	for (int i = 0; i < numScreens; i++)
	{
		/* Init the frame buffer */
		glGenFramebuffers(1, &(frameBufferNames[i]));
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferNames[i]);

		/* Init the textures */
		glGenTextures(1, &(renderedTextures[i]));
		glBindTexture(GL_TEXTURE_2D, renderedTextures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);//TODO 

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		/* Now for the depth buffer */
		glGenRenderbuffers(1, &(depthRenderBuffers[i]));
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffers[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024); //TODO
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		/* Attaching the depth buffer to the frame buffer */
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffers[i]);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextures[i], 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
			success = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	}
	return success;
}
void CaveSimulatorApp::updateControllersAction()
{
	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
		if (inputState.Thumbstick[ovrHand_Left].x < -0.500f)
		{
			cubeScene->contractCube();
		}
		else if (inputState.Thumbstick[ovrHand_Left].x > 0.500f)
		{
			cubeScene->expandCube();
		}
		else if (inputState.Buttons == ovrButton_LThumb)
		{
			cubeScene->resetCubeSize();
		}
		if (inputState.Thumbstick[ovrHand_Right].x < -0.500f)
		{
			cubeScene->translateCube(glm::vec3(-0.01f, 0.0f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Right].x > 0.500f)
		{
			cubeScene->translateCube(glm::vec3(0.01f, 0.0f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Right].y < -0.500f)
		{
			cubeScene->translateCube(glm::vec3(0.0f, -0.01f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Right].y > 0.500f)
		{
			cubeScene->translateCube(glm::vec3(0.0f, 0.01f, 0.0f));
		}
		if (inputState.Thumbstick[ovrHand_Left].y < -0.500f)
		{
			cubeScene->translateCube(glm::vec3(0.0f, 0.0f, 0.01f));
		}
		if (inputState.Thumbstick[ovrHand_Left].y > 0.500f)
		{
			cubeScene->translateCube(glm::vec3(0.0f, 0.0f, -0.01f));
		}
		if (inputState.Buttons == ovrTouch_A)
		{	
			aPressed = true;
		}
		else
		{
			aPressed = false;
		}
		if (inputState.Buttons == ovrTouch_Y)
		{
			yPressed = true;
		}
		else
		{
			yPressed = false;
		}
		if (inputState.HandTrigger[ovrHand_Right] > 0.5f)
		{
			triggerPressed = true;
		}
		else
		{
			triggerPressed = false;
		}

		if (inputState.Buttons == ovrTouch_B) {
			if (!bPressed) {
				freeze = !freeze;
				bPressed = true;
			}
		}
		else {
			bPressed = false;
		}

	}
	
}
void CaveSimulatorApp::renderToTexture(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right	

	glm::vec3 eyePos = glm::vec3(headPose[3]); // Change for the handcase 
	
	for (int i = 0; i < numScreens; i++)
	{
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferNames[i]);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!yPressed || i != 1 || !isRenderingLeft())
		{
			glm::mat4 projection = screens[i]->getProjection(eyePos);
			glUseProgram(skyBoxShaderProgram);
			if (isRenderingLeft())
			{
				leftEyeSkyBox->draw(projection, glm::inverse(headPose), skyBoxShaderProgram);
			}
			else
			{
				rightEyeSkyBox->draw(projection, glm::inverse(headPose), skyBoxShaderProgram);
			}

			glUseProgram(cubeSceneShaderProgram);
			cubeScene->render(projection, glm::inverse(headPose), cubeSceneShaderProgram);
		}
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo); //Kinda overkill
}
void CaveSimulatorApp::drawFrustums(const glm::mat4 & projection, const glm::mat4 caveEyePose, const glm::mat4 & headPose)
{
	glm::vec3 frustColor;
	if (isRenderingLeft())
	{
		frustColor = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		frustColor = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < numScreens; i++)
	{
		screens[i]->getCorners(pas[i], pbs[i], pcs[i], pds[i]);
		glm::vec3 corners[] = { pas[i], pbs[i], pcs[i], pds[i] };
		glm::vec3 eyePos = glm::vec3(caveEyePose[3]);
		
		frusts[i]->setColor(frustColor);
		frusts[i]->setEndPoints(eyePos, corners);
		frusts[i]->draw(frustumShaderProgram, projection, glm::inverse(headPose));
	}
}
glm::mat4 CaveSimulatorApp::getHeadPose(const glm::mat4 & headPose)
{
	glm::mat4 resHeadPose = headPose;
	
	if (triggerPressed) {
		//get ovr exact system time
		double displayMidpointSeconds = ovr_GetPredictedDisplayTime(_session, 0);

		//get current ovrState, contains info such as head position, hand positions
		ovrTrackingState trackState = ovr_GetTrackingState(_session, displayMidpointSeconds, ovrTrue);

		//two touch controller position, getting from trackState, each ovrPosef contains a vec4 orientation, vec3 position	
		ovrPosef handRPosef;
		handRPosef = trackState.HandPoses[ovrHand_Right].ThePose;

		quat eyeRotQuat = glm::make_quat(&(handRPosef.Orientation.x));
		glm::mat4 eyeOrientation = glm::mat4_cast(eyeRotQuat);

		glm::vec3 xOffset;

		if (isRenderingLeft()) {				
			xOffset = eyeOrientation * glm::vec4(-0.065f, 0.0f, 0.0f, 1.0f);
		}
		else {
			xOffset = eyeOrientation * glm::vec4(0.065f, 0.0f, 0.0f, 1.0f);
		}

		glm::mat4 translation = glm::translate(mat4(), glm::make_vec3(&(handRPosef.Position.x)));
		translation = glm::translate(glm::mat4(1.0f), xOffset) * translation;
		resHeadPose =  translation * eyeOrientation;	
	}	
	if (freeze)
	{
		if (isRenderingLeft())
		{
			resHeadPose = oldLeftEyePose;
		}
		else
		{
			resHeadPose = oldRightEyePose;
		}
	}
	/* Update the old poses */
	if (isRenderingLeft())
	{
		oldLeftEyePose = resHeadPose;
	}
	else
	{
		oldRightEyePose = resHeadPose;
	}
	return resHeadPose;
}

void CaveSimulatorApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	/* Load the shader program */
	if (loadedShaders == false)
	{		
		vrShaderProgram = LoadShaders(VR_VERTEX_SHADER_PATH, VR_FRAGMENT_SHADER_PATH);
		cubeSceneShaderProgram = LoadShaders(CUBE_SCENE_VERTEX_SHADER_PATH, CUBE_SCENE_FRAGMENT_SHADER_PATH);
		skyBoxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
		frustumShaderProgram = LoadShaders(VR_FRUSTUM_VERTEX_SHADER_PATH, VR_FRUSTUM_FRAGMENT_SHADER_PATH);
		loadedShaders = true;
	}

	glm::mat4 caveEyePose = getHeadPose(headPose);
	/* Now to do the first pass */
	renderToTexture(projection, caveEyePose);

	/* Reset the view port */
	glViewport(vpPosX, vpPosY, vpSizeW, vpSizeH);

	/* Now to render to the screen */
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	
	/* Render the outer skybox */
	glUseProgram(skyBoxShaderProgram);
	outerMonoSkyBox->draw(projection, glm::inverse(headPose), skyBoxShaderProgram);

	/* Now to do the second pass */
	glUseProgram(vrShaderProgram);
	for (int i = 0; i < 3; i++)
	{
		screens[i]->textureID = renderedTextures[i];
		screens[i]->draw(vrShaderProgram, projection, glm::inverse(headPose));
	}
	
	if (aPressed)
	{		
		glUseProgram(frustumShaderProgram);
		drawFrustums(projection, caveEyePose, headPose);
	}
}
void CaveSimulatorApp::shutdownGl() {
	delete(cubeScene);
}
void CaveSimulatorApp::update()
{ 
	/* only just spin the cube */
	updateControllersAction();
	cubeScene->update();
}
CaveSimulatorApp::~CaveSimulatorApp()
{
}
