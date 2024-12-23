#pragma once

#include "openGLRenderer.h"
#include "../glframework/texture.h"
#include "../glframework/shader.h"
#include "../application/camera/cameraControl.h"

class DepthTestEX : public OpenGLRenderer
{
public:
	DepthTestEX() = default;
	DepthTestEX(const DepthTestEX&) = default;
	DepthTestEX& operator=(const DepthTestEX&) = default;
	~DepthTestEX() = default;

	DepthTestEX(const Camera& _rCamera);

	void prepareVAO() override;
	void prepareShader() override;
	void prepareTexture() override;
	void render() override;

private:
	const Camera& rCamera;
	GLuint  mVao{ 0 };
	Texture mTextureGoku;
	Texture mTextureLuffy;
	Shader  mShader;
	glm::mat4 transform{ 1.0f };
	glm::mat4 transform2{ glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 0.0f, -1.0f)) };
};