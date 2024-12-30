#include "spotLightEX.h"

#include "../glframework/material/phongMaterial.h"
#include <iostream>


SpotLightEX::SpotLightEX(const Camera& _rCamera)
	: rCamera(_rCamera),
	geometry(nullptr),
	diffuseTex(nullptr),
	maskTex(nullptr),
	material0(nullptr)
{}

SpotLightEX::~SpotLightEX()
{
	printf("---- ~SpotLightEX ----\n");

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
void SpotLightEX::render()
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
			shader.setVector3("lightPosition", spotLight.getPosition());
			shader.setVector3("lightColor", spotLight.getColor());
			shader.setVector3("targetDirection", spotLight.mTargetDirection);
			shader.setFloat("specularIntensity", spotLight.getSpecularIntensity());
			shader.setFloat("innerLine", glm::cos(glm::radians(spotLight.mInnerAngle)));
			shader.setFloat("outerLine", glm::cos(glm::radians(spotLight.mOuterAngle)));
			shader.setFloat("k2", spotLight.mK2);
			shader.setFloat("k1", spotLight.mK1);
			shader.setFloat("kc", spotLight.mKc);

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

void SpotLightEX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/SpotLight.vert", "assets/shaders/SpotLight.frag");
}

void SpotLightEX::prepareScene()
{
	this->prepareShader();

	//1 ����geometry
	geometry = Geometry::createBox(2.5f);

	//2 ����һ��material
	material0 = new PhongMaterial();

	diffuseTex = new Texture();
	diffuseTex->initTexture("assets/textures/box.png", 0, true);
	maskTex = new Texture();
	maskTex->initTexture("assets/textures/sp_mask.png", 1, true);

	material0->setShiness(32.0f);
	material0->setDiffuse(diffuseTex);
	material0->setSpecularMask(maskTex);

	this->addMesh(Mesh(geometry, material0));

	spotLight.setPosition(rCamera.mPosition);
	spotLight.mTargetDirection = glm::cross(rCamera.mUp, rCamera.mRight);
	spotLight.mInnerAngle = 8.0f;
	spotLight.mOuterAngle = 16.0f;
	spotLight.mK2 = 0.032f;
	spotLight.mK1 = 0.09f;
	spotLight.mKc = 1.0f;

	ambLight.setColor(glm::vec3(0.01f));
}

// Add a mesh to the renderer
void SpotLightEX::addMesh(Mesh& mesh)
{
	meshes.push_back(mesh);
}

void SpotLightEX::doTransform()
{
	spotLight.setPosition(rCamera.mPosition);
	spotLight.mTargetDirection = glm::cross(rCamera.mUp, rCamera.mRight);
}


Shader& SpotLightEX::pickShader(MaterialType type)
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