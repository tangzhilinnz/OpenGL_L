#pragma once

#include "openGLRenderer.h"
#include "../glframework/texture.h"
#include "../glframework/shader.h"
#include "../glframework/geometry.h"
#include "../application/camera/cameraControl.h"

class SimpleLightEX : public OpenGLRenderer
{
public:
	SimpleLightEX() = default;
	SimpleLightEX(const SimpleLightEX&) = default;
	SimpleLightEX& operator=(const SimpleLightEX&) = default;
	~SimpleLightEX();
	SimpleLightEX(const Camera& _rCamera);

	void prepareVAO() override;
	void prepareShader() override;
	void prepareTexture() override;
	void render() override;

private:
	void doTransform();

	const Camera& rCamera;
	Geometry* mGeometry;
	Texture mTexture;
	Shader  mShader;
	glm::mat4 transform{ 1.0f };
	//ƽ�й⣺���������򣬹�ǿ�� uniform������ʽ
	glm::vec3 lightDirection{ 1.0f, 1.0f, -1.0f };
	glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	//specular
	float specularIntensity{ 0.8f };
	//ambient
	glm::vec3 ambientLight{ 0.2f, 0.2f, 0.2f };
};