#include "../glframework/geometry.h"
#include "../wrapper/checkError.h"
#include "geometryEX.h"

GeometryEX::~GeometryEX()
{
	printf("---- ~GeometryEX ----\n");

	if (mGeometry)
	{
		delete mGeometry;
	}
}

GeometryEX::GeometryEX(const Camera& _rCamera)
	: rCamera(_rCamera),
	mGeometry(nullptr)
{}

void GeometryEX::prepareVAO()
{
	//mGeometry = Geometry::createBox(3.0f);
	mGeometry = Geometry::createSphere(3.0f);
	//mGeometry = Geometry::createPlane(4.0f, 3.0f);
}

void GeometryEX::prepareShader()
{
	//mShader.initShader("assets/shaders/GeometryV.glsl", "assets/shaders/GeometryF.glsl");
	mShader.initShader("assets/shaders/GeometryNormalV.glsl", "assets/shaders/GeometryNormalF.glsl");
}

void GeometryEX::prepareTexture()
{
	mTexture.initTexture("assets/textures/earth.png", 0);
}

void GeometryEX::render()
{
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//�󶨵�ǰ��program
	mShader.begin();
		//mShader.setInt("sampler", 0);
		mShader.setMatrix4x4("transform", transform);
		mShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		mShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

		//�󶨵�ǰ��vao
		GL_CALL(glBindVertexArray(mGeometry->getVao()));

		//��������ָ��
		GL_CALL(glDrawElements(GL_TRIANGLES, mGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
		GL_CALL(glBindVertexArray(0));

	mShader.end();
}