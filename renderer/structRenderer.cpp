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

	//1 设置当前帧绘制的时候，opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//3 遍历mesh进行绘制
	for (size_t i = 0; i < meshes.size(); i++)
	{
		Mesh& mesh = meshes[i];
		auto geometry = mesh.getGeometry();
		auto material = mesh.getMaterial();

		//1 决定使用哪个Shader 
		Shader& shader = pickShader(material->mType);

		//2 更新shader的uniform
		shader.begin();

		switch (material->mType)
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* phongMat = (PhongMaterial*)material;
			//diffuse贴图
			//将纹理采样器与纹理单元进行挂钩
			shader.setInt("sampler", 0);

			//将纹理与纹理单元进行挂钩
			phongMat->bindDiffuse();

			//mvp
			shader.setMatrix4x4("modelMatrix", mesh.getModelMatrix());
			shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
			shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh.getModelMatrix())));
			shader.setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			shader.setVector3("lightDirection", dirLight.getDirection());
			shader.setVector3("lightColor", dirLight.getColor());
			shader.setFloat("specularIntensity", dirLight.getSpecularIntensity());

			shader.setFloat("shiness", phongMat->getShiness());

			shader.setVector3("ambientColor", ambLight.getColor());

			//相机信息更新
			shader.setVector3("cameraPosition", rCamera.mPosition);

		}
										break;
		default:
			continue;
		}

		//3 绑定vao
		glBindVertexArray(geometry->getVao());

		//4 执行绘制命令
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

	//1 创建geometry
	geometry = Geometry::createSphere(1.5f);

	//2 创建一个material
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