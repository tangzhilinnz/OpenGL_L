#pragma once

#include "openGLRenderer.h"
#include "../glframework/texture.h"
#include "../glframework/shader.h"
#include "../application/camera/cameraControl.h"

class GrassLandEX : public OpenGLRenderer
{
public:
	GrassLandEX() = default;
	GrassLandEX(const GrassLandEX&) = default;
	GrassLandEX& operator=(const GrassLandEX&) = default;
	~GrassLandEX() = default;

	GrassLandEX(const Camera& _rCamera);

	void prepareVAO() override;
	void prepareShader() override;
	void prepareTexture() override;
	void render() override;

private:
	const Camera& rCamera;
	GLuint  mVao{ 0 };
	Shader  mShader;
	Texture mGrassTexture;
	Texture mLandTexture;
	Texture mNoiseTexture;
	glm::mat4 transform{ 1.0f };
};