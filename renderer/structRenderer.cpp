#include "structRenderer.h"

#include "../glframework/material/phongMaterial.h"
#include <iostream>


StructRenderer::StructRenderer(const Camera& _rCamera)
	: rCamera(_rCamera),
	geometry(nullptr),
    texture0(nullptr),
    texture1(nullptr),
    material0(nullptr),
    material1(nullptr)
{}

StructRenderer::~StructRenderer()
{
	printf("---- ~StructRenderer ----\n");

	if (geometry)
	{
		delete geometry;
	}

	if (texture0)
	{
		delete texture0;
	}

	if (texture1)
	{
		delete texture1;
	}

	if (material0)
	{
		delete material0;
	}

	if (material1)
	{
		delete material1;
	}
}

// Methods
void StructRenderer::render()
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

void StructRenderer::prepareShader()
{
	mPhongShader.initShader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
}

void StructRenderer::prepareScene()
{
	this->prepareShader();

	//1 ����geometry
	geometry = Geometry::createSphere(1.5f);

	//2 ����һ��material
	material0 = new PhongMaterial();
	material1 = new PhongMaterial();

	texture0 =new Texture();
	texture0->initTexture("assets/textures/goku.jpg", 0, true);
	material0->setShiness(32.0f);
	material0->setDiffuse(texture0);

	texture1 = new Texture();
	texture1->initTexture("assets/textures/wall.jpg", 0, true);
	material1->setShiness(32.0f);
	material1->setDiffuse(texture1);

	this->addMesh(Mesh(geometry, material0));
	this->addMesh(Mesh(geometry, material1));

	meshes[1].setPosition(glm::vec3(4.0f, 0.0f, 0.0f));

	ambLight.setColor(glm::vec3(0.1f));
}

// Add a mesh to the renderer
void StructRenderer::addMesh(Mesh& mesh)
{
	meshes.push_back(mesh);
}

void StructRenderer::doTransform()
{
	meshes[1].rotateX(1.0f);
	meshes[1].rotateY(5.0f);
}


Shader& StructRenderer::pickShader(MaterialType type)
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