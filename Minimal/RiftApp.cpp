#include "RiftApp.h"
#include <algorithm>
#include <iostream>

RiftApp::RiftApp()
{
	using namespace ovr;
	_viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;

	memset(&_sceneLayer, 0, sizeof(ovrLayerEyeFov));
	_sceneLayer.Header.Type = ovrLayerType_EyeFov;
	_sceneLayer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

	ovr::for_each_eye([&](ovrEyeType eye) {
		ovrEyeRenderDesc& erd = _eyeRenderDescs[eye] = ovr_GetRenderDesc(_session, eye, _hmdDesc.DefaultEyeFov[eye]);
		ovrMatrix4f ovrPerspectiveProjection =
			ovrMatrix4f_Projection(erd.Fov, 0.01f, 2000.0f, ovrProjection_ClipRangeOpenGL);
		_eyeProjections[eye] = ovr::toGlm(ovrPerspectiveProjection);
		_viewScaleDesc.HmdToEyeOffset[eye] = erd.HmdToEyeOffset;

		ovrFovPort & fov = _sceneLayer.Fov[eye] = _eyeRenderDescs[eye].Fov;
		auto eyeSize = ovr_GetFovTextureSize(_session, eye, fov, 1.0f);
		_sceneLayer.Viewport[eye].Size = eyeSize;
		_sceneLayer.Viewport[eye].Pos = { (int)_renderTargetSize.x, 0 };

		_renderTargetSize.y = std::max(_renderTargetSize.y, (uint32_t)eyeSize.h);
		_renderTargetSize.x += eyeSize.w;
	});
	// Make the on screen window 1/4 the resolution of the render target
	_mirrorSize = _renderTargetSize;
	_mirrorSize /= 4;
}
GLFWwindow * RiftApp::createRenderingTarget(uvec2 & outSize, ivec2 & outPosition)
{
	return glfw::createWindow(_mirrorSize);
}
void RiftApp::initGl() {
	GlfwApp::initGl();

	// Disable the v-sync for buffer swap
	glfwSwapInterval(0);

	ovrTextureSwapChainDesc desc = {};
	desc.Type = ovrTexture_2D;
	desc.ArraySize = 1;
	desc.Width = _renderTargetSize.x;
	desc.Height = _renderTargetSize.y;
	desc.MipLevels = 1;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleCount = 1;
	desc.StaticImage = ovrFalse;
	ovrResult result = ovr_CreateTextureSwapChainGL(_session, &desc, &_eyeTexture);
	_sceneLayer.ColorTexture[0] = _eyeTexture;
	if (!OVR_SUCCESS(result)) {
		FAIL("Failed to create swap textures");
	}
	int length = 0;
	result = ovr_GetTextureSwapChainLength(_session, _eyeTexture, &length);
	if (!OVR_SUCCESS(result) || !length) {
		FAIL("Unable to count swap chain textures");
	}
	for (int i = 0; i < length; ++i) {
		GLuint chainTexId;
		ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, i, &chainTexId);
		glBindTexture(GL_TEXTURE_2D, chainTexId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set up the framebuffer object
	glGenFramebuffers(1, &_fbo);
	glGenRenderbuffers(1, &_depthBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _renderTargetSize.x, _renderTargetSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	ovrMirrorTextureDesc mirrorDesc;
	memset(&mirrorDesc, 0, sizeof(mirrorDesc));
	mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	mirrorDesc.Width = _mirrorSize.x;
	mirrorDesc.Height = _mirrorSize.y;
	if (!OVR_SUCCESS(ovr_CreateMirrorTextureGL(_session, &mirrorDesc, &_mirrorTexture))) {
		FAIL("Could not create mirror texture");
	}
	glGenFramebuffers(1, &_mirrorFbo);

	renderedFirst = false;
}
void RiftApp::onKey(int key, int scancode, int action, int mods) {
	if (GLFW_PRESS == action) switch (key) {
	case GLFW_KEY_R:
		ovr_RecenterTrackingOrigin(_session);
		return;
	}

	GlfwApp::onKey(key, scancode, action, mods);
}
void RiftApp::draw()
{
	
	ovrPosef eyePoses[2];
	ovr_GetEyePoses(_session, frame, true, _viewScaleDesc.HmdToEyeOffset, eyePoses, &_sceneLayer.SensorSampleTime);
	
	
	if (!renderedFirst)
	{
		rendEyePoses[0] = eyePoses[0];
		rendEyePoses[1] = eyePoses[1];
		

		/* Offset to go on top of the city */
		glm::vec3 headPos = (ovr::toGlm(eyePoses[0].Position) + ovr::toGlm(eyePoses[1].Position)) / 2.0f;
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 10.0f, 40.0f) - headPos);
		
		glm::vec3 leftPos = glm::vec3(trans * glm::vec4(ovr::toGlm(rendEyePoses[0].Position), 1.0f));
		glm::vec3 rightPos = glm::vec3(trans * glm::vec4(ovr::toGlm(rendEyePoses[1].Position), 1.0f));
		rendEyePoses[0].Position = ovr::fromGlm(leftPos);
		rendEyePoses[1].Position = ovr::fromGlm(rightPos);
		
		renderedFirst = true;
	}
	
	/* Always tracking the orientation */
	rendEyePoses[0].Orientation = eyePoses[0].Orientation;
	rendEyePoses[1].Orientation = eyePoses[1].Orientation;	

	int curIndex;
	ovr_GetTextureSwapChainCurrentIndex(_session, _eyeTexture, &curIndex);
	GLuint curTexId;
	ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, curIndex, &curTexId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curTexId, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ovr::for_each_eye([&](ovrEyeType eye) {
		const auto& vp = _sceneLayer.Viewport[eye];
		glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
		vpPosX = vp.Pos.x; vpPosY = vp.Pos.y; vpSizeW = vp.Size.w; vpSizeH = vp.Size.h;

		_sceneLayer.RenderPose[eye] = rendEyePoses[eye];
		if (eye == ovrEye_Left)
		{
			renderingLeft = true;
		}
		else
		{
			renderingLeft = false;
		}
		renderScene(_eyeProjections[eye], ovr::toGlm(rendEyePoses[eye]));
	});
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	ovr_CommitTextureSwapChain(_session, _eyeTexture);
	ovrLayerHeader* headerList = &_sceneLayer.Header;
	ovr_SubmitFrame(_session, frame, &_viewScaleDesc, &headerList, 1);

	GLuint mirrorTextureId;
	ovr_GetMirrorTextureBufferGL(_session, _mirrorTexture, &mirrorTextureId);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureId, 0);
	glBlitFramebuffer(0, 0, _mirrorSize.x, _mirrorSize.y, 0, _mirrorSize.y, _mirrorSize.x, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
bool RiftApp::isRenderingLeft()
{
	return renderingLeft;
}
RiftApp::~RiftApp()
{
}
