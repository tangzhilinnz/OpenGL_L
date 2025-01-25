#include "../glframework/geometry.h"
#include "../wrapper/checkError.h"
#include "simpleLightEX.h"

SimpleLightEX::~SimpleLightEX()
{
	printf("---- ~SimpleLightEX ----\n");

	if (mGeometry)
	{
		delete mGeometry;
	}
}

SimpleLightEX::SimpleLightEX(const Camera& _rCamera)
	: rCamera(_rCamera),
	mGeometry(nullptr)
{}

void SimpleLightEX::prepareVAO()
{
	//mGeometry = Geometry::createBox(3.0f);
	mGeometry = Geometry::createSphere(3.0f);
}

void SimpleLightEX::prepareShader()
{
	mShader.initShader("assets/shaders/SimpleLightV.glsl", "assets/shaders/SimpleLightF.glsl");
}

void SimpleLightEX::prepareTexture()
{
	mTexture.initTexture("assets/textures/earth.png", 0);
}

void SimpleLightEX::render()
{
	this->doTransform();

	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//绑定当前的program
	mShader.begin();
		mShader.setInt("sampler", 0);
		mShader.setMatrix4x4("modelMatrix", transform);
		mShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		mShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());
		//计算NormalMatrix
		auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		mShader.setMatrix3x3("normalMatrix", normalMatrix);

		//光源参数的uniform更新
		mShader.setVector3("lightDirection", lightDirection);
		mShader.setVector3("lightColor", lightColor);
		mShader.setFloat("specularIntensity", specularIntensity);
		mShader.setVector3("ambientLight", ambientLight);
		mShader.setVector3("cameraPosition", rCamera.mPosition);

		//绑定当前的vao
		GL_CALL(glBindVertexArray(mGeometry->getVao()));

		//发出绘制指令
		GL_CALL(glDrawElements(GL_TRIANGLES, mGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
		GL_CALL(glBindVertexArray(0));

	mShader.end();
}

void SimpleLightEX::doTransform()
{
	this->transform = glm::rotate(transform, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
}