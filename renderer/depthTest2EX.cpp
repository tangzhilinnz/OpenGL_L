#include "depthTest2EX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/depthMaterial.h"

#include <iostream>
#include <string>


DepthTest2EX::DepthTest2EX(const Camera& _rCamera)
	: rCamera(_rCamera)
{}

DepthTest2EX::~DepthTest2EX()
{
	printf("---- ~DepthTest2EX ----\n");
	RenderTool::sceneClear();
}

void DepthTest2EX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/readingModel.vert", "assets/shaders/readingModel.frag");
	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
	mDepthShader.initShader("assets/shaders/Depth.vert", "assets/shaders/Depth.frag");
}

void DepthTest2EX::prepareScene()
{
	this->prepareShader();

	scene = Object::createObj();
	auto geometry = Geometry::createPlane(600.0f, 900.0f);

	//auto materialA = DepthMaterial::createMaterial();
	auto materialA = PhongMaterial::createMaterial();
	materialA->setDiffuse(Texture::createTexture("assets/textures/goku.jpg", 0));
	materialA->setSpecularMask(Texture::createTexture("assets/textures/zero_specular_mask.png", 1));

	auto meshA = Mesh::createObj(geometry, materialA);
	meshA->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	meshA->rotateX(-88.0f);

	scene->addChild(meshA);

	auto materialB = PhongMaterial::createMaterial();
	materialB->setDiffuse(Texture::createTexture("assets/textures/box.png", 0));
	materialB->setSpecularMask(Texture::createTexture("assets/textures/zero_specular_mask.png", 1));

	auto meshB = Mesh::createObj(geometry, materialB);
	meshB->setPosition(glm::vec3(100.0f, 0.0f, -1.5f));
	meshB->rotateX(-88.0f);
	meshB->disableDepthWrite();
	meshB->enablePolygonOffsetFill();
	meshB->polygonOffset(1.0f, 1.0f);
	scene->addChild(meshB);

	//auto materialC = PhongMaterial::createMaterial();
	//materialC->setDiffuse(Texture::createTexture("assets/textures/earth.png", 0));
	//materialC->setSpecularMask(Texture::createTexture("assets/textures/defaultTexture.jpg", 1));
	//auto meshC = Mesh::createObj(geometry, materialA);
	//meshC->setPosition(glm::vec3(4.0f, 1.0f, -2.0f));
	//scene->addChild(meshC);

	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setSpecularIntensity(0.01f);
	ambLight.setColor(glm::vec3(0.2f));
}

void DepthTest2EX::render()
{
	this->doTransform();

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//���õ�ǰ֡���Ƶ�ʱ��opengl�ı�Ҫ״̬������
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glDepthMask(GL_TRUE));

	GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
	GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));

	//�������ԡ����û���д��״̬����ģ�����д��
	GL_CALL(glEnable(GL_STENCIL_TEST));
	GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	GL_CALL(glStencilMask(0xFF));//��֤��ģ�建����Ա�����

	//Ĭ����ɫ���
	GL_CALL(glDisable(GL_BLEND));

	//������ 
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	RenderTool::objectRender(scene, [this](Object* obj) { this->meshRendering(obj); });
}

void DepthTest2EX::meshRendering(Object* object)
{
	Mesh* mesh = (Mesh*)object;
	Geometry* geometry = mesh->getGeometry();
	Material* material = mesh->getMaterial();

	//������Ⱦ״̬
	mesh->applyState();

	//1 ����ʹ���ĸ�Shader 
	Shader& shader = pickShader(material->mType);

	//2 ����shader��uniform
	shader.begin();

	switch (material->mType) {
	case MaterialType::PhongMaterial: {
		PhongMaterial* phongMat = (PhongMaterial*)material;

		//diffuse��ͼ֡����
		//�����������������Ԫ���йҹ�
		shader.setInt("sampler", 0);
		//������������Ԫ���йҹ�
		phongMat->bindDiffuse();

		//�߹��ɰ��֡����
		shader.setInt("specularMaskSampler", 1);
		phongMat->bindSpecularMask();

		//mvp
		shader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

		auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
		shader.setMatrix3x3("normalMatrix", normalMatrix);

		//��Դ������uniform����
		//directionalLight �ĸ���
		shader.setVector3("directionalLight.color", dirLight.getColor());
		shader.setVector3("directionalLight.direction", dirLight.getDirection());
		shader.setFloat("directionalLight.specularIntensity", dirLight.getSpecularIntensity());

		shader.setFloat("shiness", phongMat->getShiness());

		shader.setVector3("ambientLight.color", ambLight.getColor());

		//�����Ϣ����
		shader.setVector3("cameraPosition", rCamera.mPosition);
	}
	    break;
	case MaterialType::WhiteMaterial: {
		//mvp
		shader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());
	}
	    break;
	case MaterialType::DepthMaterial: {
		//mvp
		shader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

		shader.setFloat("near", rCamera.mNear);
		shader.setFloat("far", rCamera.mFar);
	}
		break;
	default:
		break;
	}

	//3 ��vao
	glBindVertexArray(geometry->getVao());

	//4 ִ�л�������
	glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

	shader.end();
}


Shader& DepthTest2EX::pickShader(MaterialType type)
{
	switch (type)
	{
	case MaterialType::PhongMaterial:
		return mPhongShader;
	case MaterialType::WhiteMaterial:
		return mWhiteShader;
	case MaterialType::DepthMaterial:
		return mDepthShader;
	default:
		std::cerr << "Unknown material type to pick shader" << std::endl;
		exit(-1);
		break;
	}
}