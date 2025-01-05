#include "stencilTest2EX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/depthMaterial.h"
#include "../glframework/material/whiteMaterial.h"

#include <iostream>
#include <string>


StencilTest2EX::StencilTest2EX(const Camera& _rCamera)
	: rCamera(_rCamera)
{}

StencilTest2EX::~StencilTest2EX()
{
	printf("---- ~StencilTest2EX ----\n");
	RenderTool::sceneClear();
}

void StencilTest2EX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/readingModel.vert", "assets/shaders/readingModel.frag");
	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
	mDepthShader.initShader("assets/shaders/Depth.vert", "assets/shaders/Depth.frag");
}

void StencilTest2EX::prepareScene()
{
	this->prepareShader();

	scene = Object::createObj();
	auto geometry = Geometry::createBox(4);

	auto glState = State::createState();
	glState->setStencilTest(true, GL_ALWAYS, 1, 0xFF);
	glState->setStencilOps(GL_KEEP, GL_KEEP, GL_REPLACE);
	glState->setStencilMask(0xFF);

	auto glStateBound = State::createState();
	glStateBound->setDepthTest(false);
	glStateBound->setStencilTest(true, GL_NOTEQUAL, 1, 0xFF);
	glStateBound->setStencilOps(GL_KEEP, GL_KEEP, GL_KEEP);
	glStateBound->setStencilMask(0x00);

	//---------A �����ʵ����߽�------------
	//1 ����һ����ͨ����
	auto materialA = PhongMaterial::createMaterial();
	materialA->setDiffuse(Texture::createTexture("assets/textures/goku.jpg", 0));
	materialA->setSpecularMask(Texture::createTexture("assets/textures/defaultTexture.jpg", 1));
	auto meshA = Mesh::createObj(geometry, materialA);
	meshA->setGLState(glState);
	scene->addChild(meshA);

	//2 ����һ�����߷���
	auto materialABound = WhiteMaterial::createMaterial();;
	auto meshABound = Mesh::createObj(geometry, materialABound);
	meshABound->setPosition(meshA->getPosition());
	meshABound->setScale(glm::vec3(1.2f));
	meshABound->setGLState(glStateBound);
	scene->addChild(meshABound);

	//---------B �����ʵ����߽�------------
	//1 ����һ����ͨ����
	auto materialB = PhongMaterial::createMaterial();
	materialB->setDiffuse(Texture::createTexture("assets/textures/wall.jpg", 0));
	materialB->setSpecularMask(Texture::createTexture("assets/textures/defaultTexture.jpg", 1));
	auto meshB = Mesh::createObj(geometry, materialB);
	meshB->setPosition(glm::vec3(4.5f, 1.0f, 1.0f));
	meshB->setGLState(glState);
	scene->addChild(meshB);

	//2 ����һ�����߷���
	auto meshBBound = Mesh::createObj(geometry, materialABound);
	meshBBound->setPosition(meshB->getPosition());
	meshBBound->setScale(glm::vec3(1.2f));
	meshBBound->setGLState(glStateBound);
	scene->addChild(meshBBound);

	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setSpecularIntensity(0.001f);
	ambLight.setColor(glm::vec3(0.2f));
}

void StencilTest2EX::render()
{
	this->doTransform();

	//���õ�ǰ֡���Ƶ�ʱ��opengl�ı�Ҫ״̬������
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	//�������ԡ����û���д��״̬����ģ�����д��
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);//��֤��ģ�建����Ա�����

	//������ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//��scene�������ڵ㿪iteratie��Ⱦ
	RenderTool::objectRender(scene, this);
}

void StencilTest2EX::meshRendering(Object* object)
{
	Mesh* mesh = (Mesh*)object;
	Geometry* geometry = mesh->getGeometry();
	Material* material = mesh->getMaterial();

	//������Ⱦ״̬
	State* state = mesh->getGLState();
	if (state)
	{
		state->applyDepthState();
		state->applyPolygonOffsetState();
		state->applyStencilState();
	}

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


Shader& StencilTest2EX::pickShader(MaterialType type)
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