#include "imguiEX.h"

#include "../glframework/material/phongMaterial.h"
#include <iostream>
#include <string>


ImguiEX::ImguiEX(const Camera& _rCamera)
	: rCamera(_rCamera),
	geometry(nullptr),
	diffuseTex(nullptr),
	maskTex(nullptr),
	material0(nullptr),
	geometryWhite(nullptr),
	materialWhite(nullptr)
{}

ImguiEX::~ImguiEX()
{
	printf("---- ~ImguiEX ----\n");

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
void ImguiEX::render()
{
	this->doTransform();

	////1 设置当前帧绘制的时候，opengl的必要状态机参数
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

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

				//高光蒙版的帧更新
				shader.setInt("specularMaskSampler", 1);
				phongMat->bindSpecularMask();

				//mvp
				shader.setMatrix4x4("modelMatrix", mesh.getModelMatrix());
				shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
				shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

				glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh.getModelMatrix())));
				shader.setMatrix3x3("normalMatrix", normalMatrix);

				//光源参数的uniform更新
				//spotlight的更新
				shader.setVector3("spotLight.position", spotLight.getPosition());
				shader.setVector3("spotLight.color", spotLight.getColor());
				shader.setVector3("spotLight.targetDirection", spotLight.mTargetDirection);
				shader.setFloat("spotLight.specularIntensity", spotLight.getSpecularIntensity());
				shader.setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight.mInnerAngle)));
				shader.setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight.mOuterAngle)));
				shader.setFloat("spotLight.k2", spotLight.mK2);
				shader.setFloat("spotLight.k1", spotLight.mK1);
				shader.setFloat("spotLight.kc", spotLight.mKc);

				//directionalLight 的更新
				shader.setVector3("directionalLight.color", dirLight.getColor());
				shader.setVector3("directionalLight.direction", dirLight.mDirection);
				shader.setFloat("directionalLight.specularIntensity", dirLight.getSpecularIntensity());

				//pointLight的更新
				for (int i = 0; i < sizeof(pointLights) /sizeof(pointLights[0]); i++)
				{
					auto pointLight = pointLights[i];

					std::string baseName = "pointLights[";
					baseName.append(std::to_string(i));
					baseName.append("]");

					shader.setVector3((baseName + ".color").c_str(), pointLight.getColor());
					shader.setVector3((baseName + ".position").c_str(), pointLight.getPosition());
					shader.setFloat((baseName + ".specularIntensity").c_str(), pointLight.getSpecularIntensity());
					shader.setFloat((baseName + ".k2").c_str(), pointLight.mK2);
					shader.setFloat((baseName + ".k1").c_str(), pointLight.mK1);
					shader.setFloat((baseName + ".kc").c_str(), pointLight.mKc);
				}

				//ambientLight的更新
				shader.setVector3("ambientLight.color", ambLight.getColor());

				shader.setFloat("shiness", phongMat->getShiness());

				//相机信息更新
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

			//3 绑定vao
			glBindVertexArray(geometry->getVao());

			//4 执行绘制命令
			glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

		shader.end();
	}
}

void ImguiEX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/Imgui.vert", "assets/shaders/Imgui.frag");
	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
}

void ImguiEX::prepareScene()
{
	this->prepareShader();

	//1 创建geometry
	geometry = Geometry::createBox(2.5f);

	//2 创建一个material
	material0 = new PhongMaterial();

	diffuseTex = new Texture();
	diffuseTex->initTexture("assets/textures/box.png", 0, true);
	maskTex = new Texture();
	maskTex->initTexture("assets/textures/sp_mask.png", 1, true);

	material0->setShiness(32.0f);
	material0->setDiffuse(diffuseTex);
	material0->setSpecularMask(maskTex);

	this->addMesh(Mesh(geometry, material0));

	//创建白色物体
	geometryWhite = Geometry::createSphere(0.1f);
	materialWhite = new WhiteMaterial();
	this->addMesh(Mesh(geometryWhite, materialWhite));
	meshes[meshes.size() - 1].setPosition(glm::vec3(0.0f, 0.0f, 2.0f));

	spotLight.setPosition(rCamera.mPosition);
	spotLight.mTargetDirection = glm::cross(rCamera.mUp, rCamera.mRight);
	spotLight.mInnerAngle = 8.0f;
	spotLight.mOuterAngle = 16.0f;
	spotLight.mK2 = 0.032f;
	spotLight.mK1 = 0.09f;
	spotLight.mKc = 1.0f;

	dirLight.mDirection = glm::vec3(1.0f);

	ambLight.setColor(glm::vec3(0.1f));

	//创造pointLights数据
	pointLights[0].setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	pointLights[0].setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	pointLights[0].mK2 = 0.2f;
	pointLights[0].mK1 = 0.22f;
	pointLights[0].mKc = 1.0f;

	pointLights[1].setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	pointLights[1].setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	pointLights[1].mK2 = 0.0f;
	pointLights[1].mK1 = 0.0f;
	pointLights[1].mKc = 1.0f;

	pointLights[2].setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	pointLights[2].setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	pointLights[2].mK2 = 0.0f;
	pointLights[2].mK1 = 0.0f;
	pointLights[2].mKc = 1.0f;

	pointLights[3].setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
	pointLights[3].setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	pointLights[3].mK2 = 0.0f;
	pointLights[3].mK1 = 0.0f;
	pointLights[3].mKc = 1.0f;
}

// Add a mesh to the renderer
void ImguiEX::addMesh(Mesh& mesh)
{
	meshes.push_back(mesh);
}

void ImguiEX::doTransform()
{
	float xPos = glm::sin((float)glfwGetTime()) * 1.0f + 3.0f;
	meshes[meshes.size() - 1].setPosition(glm::vec3(xPos, 0.0f, 0.0f));
	pointLights[0].setPosition(glm::vec3(xPos, 0.0f, 0.0f));

	spotLight.setPosition(rCamera.mPosition);
	spotLight.mTargetDirection = glm::cross(rCamera.mUp, rCamera.mRight);
}


Shader& ImguiEX::pickShader(MaterialType type)
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