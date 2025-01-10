#include "fboTestEX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/depthMaterial.h"
#include "../glframework/material/whiteMaterial.h"
#include "../glframework/material/screenMaterial.h"

#include <iostream>
#include <string>


FboTestEX::FboTestEX(const Camera& _rCamera)
	: rCamera(_rCamera)
{
	pCamera = &rCamera;
}

FboTestEX::~FboTestEX()
{
	printf("---- ~FboTestEX ----\n");
	RenderTool::sceneClear();
}

void FboTestEX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/PhongOpcityMask.vert", "assets/shaders/PhongOpcityMask.frag");
	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
	mDepthShader.initShader("assets/shaders/Depth.vert", "assets/shaders/Depth.frag");
	mScreenShader.initShader("assets/shaders/Screen.vert", "assets/shaders/Screen.frag");
}

void FboTestEX::prepareScene()
{
	this->prepareShader();
	
	scene = Object::createObj();
	sceneOff = Object::createObj();

	framebuffer = Framebuffer::createFramebuffer(800, 600, 0);

	//������Ⱦ��box
	auto boxGeo = Geometry::createBox(5.0f);
	auto boxMat = PhongMaterial::createMaterial();
	boxMat->setDiffuse(Texture::createTexture("assets/textures/luffy.jpg", 0));
	auto boxMesh = Mesh::createObj(boxGeo, boxMat);
	sceneOff->addChild(boxMesh);

	//������Ļ�ϵľ���
	auto geo = Geometry::createScreenPlane();
	//auto tex = Texture::createTexture("assets/textures/grassColor.jpg", 0);
	auto mat = ScreenMaterial::createMaterial(nullptr);
	auto screenMesh = Mesh::createObj(geo, mat);

	scene->addChild(screenMesh);

	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setSpecularIntensity(1.0f);
	ambLight.setColor(glm::vec3(0.2f));
}

void FboTestEX::render()
{
	this->doTransform();

	//RenderTool::objectRender(sceneOff, this, framebuffer->getFBO());
	//RenderTool::objectRender(scene, this);

	RenderTool::objectSortedRender(sceneOff, this, framebuffer->getFBO());
	RenderTool::objectSortedRender(scene, this);
}

void FboTestEX::meshRendering(Object* object)
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

		//opacityMask��֡����
		shader.setInt("opacityMaskSampler", 2);
		phongMat->bindOpcityMask();

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

		//͸����
		shader.setFloat("opacity", material->getOpacity());
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
	case MaterialType::ScreenMaterial: {
		ScreenMaterial* screenMat = (ScreenMaterial*)material;
		framebuffer->bindColorAttachment();
		shader.setInt("screenTexSampler", framebuffer->getUnit());
		//screenMat->bindScreenTex();
		//shader.setFloat("texWidth", framebuffer->getWidth());
		//shader.setFloat("texHeight", framebuffer->getHeight());
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


Shader& FboTestEX::pickShader(MaterialType type)
{
	switch (type)
	{
	case MaterialType::PhongMaterial:
		return mPhongShader;
	case MaterialType::WhiteMaterial:
		return mWhiteShader;
	case MaterialType::DepthMaterial:
		return mDepthShader;
	case MaterialType::ScreenMaterial:
		return mScreenShader;
	default:
		std::cerr << "Unknown material type to pick shader" << std::endl;
		exit(-1);
		break;
	}
}