#include "specularMask.h"

#include "../glframework/material/phongMaterial.h"
#include <iostream>


SpecularMask::SpecularMask(const Camera& _rCamera)
	: rCamera(_rCamera),
	geometry(nullptr),
    diffuseTex(nullptr),
	maskTex(nullptr),
    material0(nullptr)
{}

SpecularMask::~SpecularMask()
{
	printf("---- ~SpecularMask ----\n");

	if (geometry)
	{
		delete geometry;
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
}

// Methods
void SpecularMask::render()
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
			shader.setVector3("lightDirection", dirLight.getDirection());
			shader.setVector3("lightColor", dirLight.getColor());
			shader.setFloat("specularIntensity", dirLight.getSpecularIntensity());

			shader.setFloat("shiness", phongMat->getShiness());

			shader.setVector3("ambientColor", ambLight.getColor());

			//�����Ϣ����
			shader.setVector3("cameraPosition", rCamera.mPosition);
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

void SpecularMask::prepareShader()
{
	mPhongShader.initShader("assets/shaders/SpecularMask.vert", "assets/shaders/SpecularMask.frag");
}

void SpecularMask::prepareScene()
{
	this->prepareShader();

	//1 ����geometry
	geometry = Geometry::createBox(1.5f);

	//2 ����һ��material
	material0 = new PhongMaterial();

	diffuseTex =new Texture();
	diffuseTex->initTexture("assets/textures/box.png", 0, true);
	maskTex = new Texture();
	maskTex->initTexture("assets/textures/sp_mask.png", 1, true);

	material0->setShiness(16.0f);
	material0->setDiffuse(diffuseTex);
	material0->setSpecularMask(maskTex);

	this->addMesh(Mesh(geometry, material0));

	dirLight.setDirection(glm::vec3(-1.0f, 0.0f, -1.0f));
	ambLight.setColor(glm::vec3(0.25f));
}

// Add a mesh to the renderer
void SpecularMask::addMesh(Mesh& mesh)
{
	meshes.push_back(mesh);
}

void SpecularMask::doTransform()
{
}


Shader& SpecularMask::pickShader(MaterialType type)
{
	switch (type)
	{
	case MaterialType::PhongMaterial:
		return mPhongShader;
	default:
		std::cerr << "Unknown material type to pick shader" << std::endl;
		exit(-1);
		break;
	}
}