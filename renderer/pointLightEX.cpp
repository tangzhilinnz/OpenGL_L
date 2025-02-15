#include "pointLightEX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/whiteMaterial.h"
#include <iostream>


PointLightEX::PointLightEX(const Camera& _rCamera)
	: rCamera(_rCamera),
	geometry(nullptr),
	diffuseTex(nullptr),
	maskTex(nullptr),
	material0(nullptr),
	geometryWhite(nullptr),
	materialWhite(nullptr)
{}

PointLightEX::~PointLightEX()
{
	printf("---- ~PointLight ----\n");

	if (geometry)
	{
		delete geometry;
	}
	if (geometryWhite)
	{
		delete geometryWhite;
	}

	if (diffuseTex)
	{
		delete diffuseTex;
	}

	if (maskTex)
	{
		delete maskTex;
	}

	if (material0)
	{
		delete material0;
	}

	if (materialWhite)
	{
		delete materialWhite;
	}
}

// Methods
void PointLightEX::render()
{
	this->doTransform();

	//1 ���õ�ǰ֡���Ƶ�ʱ��opengl�ı�Ҫ״̬������
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 ������ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//3 ����mesh���л���
	for (size_t i = 0; i < meshes.size(); i++)
	{
		Mesh& mesh = meshes[i];
		auto geometry = mesh.getGeometry();
		auto material = mesh.getMaterial();

		//1 ����ʹ���ĸ�Shader 
		Shader& shader = pickShader(material->mType);

		//2 ����shader��uniform
		shader.begin();

		switch (material->mType)
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* phongMat = (PhongMaterial*)material;
			//diffuse��ͼ
			//�����������������Ԫ���йҹ�
			shader.setInt("sampler", 0);

			//������������Ԫ���йҹ�
			phongMat->bindDiffuse();

			//�߹��ɰ��֡����
			shader.setInt("specularMaskSampler", 1);
			phongMat->bindSpecularMask();

			//mvp
			shader.setMatrix4x4("modelMatrix", mesh.getModelMatrix());
			shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
			shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh.getModelMatrix())));
			shader.setMatrix3x3("normalMatrix", normalMatrix);

			//��Դ������uniform����
			shader.setVector3("lightPosition", pointLight.getPosition());
			shader.setVector3("lightColor", pointLight.getColor());
			shader.setFloat("specularIntensity", pointLight.getSpecularIntensity());
			shader.setFloat("k2", pointLight.mK2);
			shader.setFloat("k1", pointLight.mK1);
			shader.setFloat("kc", pointLight.mKc);

			shader.setFloat("shiness", phongMat->getShiness());

			shader.setVector3("ambientColor", ambLight.getColor());

			//�����Ϣ����
			shader.setVector3("cameraPosition", rCamera.mPosition);
		}
		    break;
		case MaterialType::WhiteMaterial:
		{
			//mvp
			shader.setMatrix4x4("modelMatrix", mesh.getModelMatrix());
			shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
			shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());
		}
		    break;
		default:
			continue;
		}

		//3 ��vao
		glBindVertexArray(geometry->getVao());

		//4 ִ�л�������
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}

void PointLightEX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/PointLight.vert", "assets/shaders/PointLight.frag");
	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
}

void PointLightEX::prepareScene()
{
	this->prepareShader();

	//1 ����geometry
	geometry = Geometry::createBox(1.5f);

	//2 ����һ��material
	material0 = new PhongMaterial();

	diffuseTex = new Texture();
	diffuseTex->initTexture("assets/textures/box.png", 0, true);
	maskTex = new Texture();
	maskTex->initTexture("assets/textures/sp_mask.png", 1, true);

	material0->setShiness(16.0f);
	material0->setDiffuse(diffuseTex);
	material0->setSpecularMask(maskTex);

	this->addMesh(Mesh(geometry, material0));

	//������ɫ����
	geometryWhite = Geometry::createSphere(0.1f);
	materialWhite = new WhiteMaterial();
	this->addMesh(Mesh(geometryWhite, materialWhite));
	meshes[meshes.size() - 1].setPosition(glm::vec3(2.0f, 0.0f, 0.0f));

	pointLight.setPosition(meshes[meshes.size() - 1].getPosition());
	pointLight.mK2 = 0.2f;
	pointLight.mK1 = 0.22f;
	pointLight.mKc = 1.0f;

	ambLight.setColor(glm::vec3(0.2f));
}

// Add a mesh to the renderer
void PointLightEX::addMesh(Mesh& mesh)
{
	meshes.push_back(mesh);
}

void PointLightEX::doTransform()
{
	float xPos = glm::sin((float)glfwGetTime()) * 2.0f + 3.0f;
	meshes[meshes.size() - 1].setPosition(glm::vec3(xPos, 0.0f, 0.0f));
	pointLight.setPosition(glm::vec3(xPos, 0.0f, 0.0f));
}


Shader& PointLightEX::pickShader(MaterialType type)
{
	switch (type)
	{
	case MaterialType::PhongMaterial:
		return mPhongShader;
	case MaterialType::WhiteMaterial:
		return mWhiteShader;
	default:
		std::cerr << "Unknown material type to pick shader" << std::endl;
		exit(-1);
		break;
	}
}