#pragma once

#include "openGLRenderer.h"
#include "../glframework/texture.h"
#include "../glframework/shader.h"
#include "../glframework/geometry.h"
#include "../application/camera/cameraControl.h"

class GeometryEX : public OpenGLRenderer
{
public:
	GeometryEX() = default;
	GeometryEX(const GeometryEX&) = default;
	GeometryEX& operator=(const GeometryEX&) = default;
	~GeometryEX();
	GeometryEX(const Camera& _rCamera);

	void prepareVAO() override;
	void prepareShader() override;
	void prepareTexture() override;
	void render() override;

private:
	const Camera& rCamera;
	Geometry* mGeometry;
	Texture mTexture;
	Shader  mShader;
	glm::mat4 transform{ 1.0f };
};