#pragma once

#include "openGLRenderer.h"
#include "../glframework/texture.h"
#include "../glframework/shader.h"
#include "../application/camera/cameraControl.h"

class CameraSystemEX : public OpenGLRenderer
{
public:
	CameraSystemEX() = default;
	CameraSystemEX(const CameraSystemEX&) = default;
	CameraSystemEX& operator=(const CameraSystemEX&) = default;
	~CameraSystemEX() = default;

	CameraSystemEX(const Camera& _rCamera);

	void prepareVAO() override;
	void prepareShader() override;
	void prepareTexture() override;
	void render() override;

private:
	const Camera& rCamera;
	GLuint  mVao{ 0 };
	Texture mTexture;
	Shader  mShader;
	glm::mat4 transform{ 1.0f };
};