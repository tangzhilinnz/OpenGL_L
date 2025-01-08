#include "blendTestEX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/depthMaterial.h"
#include "../glframework/material/whiteMaterial.h"

#include <iostream>
#include <string>


BlendTestEX::BlendTestEX(const Camera& _rCamera)
	: rCamera(_rCamera)
{
	pCamera = &rCamera;
}

BlendTestEX::~BlendTestEX()
{
	printf("---- ~BlendTestEX ----\n");
	RenderTool::sceneClear();
}

void BlendTestEX::prepareShader()
{
	mPhongShader.initShader("assets/shaders/PhongOpcityMask.vert", "assets/shaders/PhongOpcityMask.frag");
	//mPhongShader.initShader("assets/shaders/Blend.vert", "assets/shaders/Blend.frag");
	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
	mDepthShader.initShader("assets/shaders/Depth.vert", "assets/shaders/Depth.frag");
}

void BlendTestEX::prepareScene()
{
	this->prepareShader();
	
	scene = Object::createObj();

	////1 背包模型
	//auto backpack = AssimpLoader::load("assets/fbx/bag/backpack.obj");
	//RenderTool::enableModelBlend(backpack);
	//RenderTool::setModelOpcity(backpack, 0.3f);
	////RenderTool::disableModelBlend(scene);
	//scene->addChild(backpack);

	//grass model
	//auto grassGeo = Geometry::createPlane(5.0f, 5.0f);
	//auto grassMat = PhongMaterial::createMaterial();
	//grassMat->setDiffuse(Texture::createTexture("assets/textures/grassColor.jpg", 0));
	//grassMat->setOpcityMask(Texture::createTexture("assets/textures/grassMask.png", 2));
	//auto grassMesh = Mesh::createObj(grassGeo, grassMat);
	//grassMesh->enableBlend();
	//grassMesh->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//scene->addChild(grassMesh);

	auto grassModel = AssimpLoader::load("assets/fbx/grass.fbx");
	grassModel->setScale(glm::vec3(0.02f));
	auto grassMat = PhongMaterial::createMaterial();
	grassMat->setDiffuse(Texture::createTexture("assets/textures/grassColor.jpg", 0));
	grassMat->setOpcityMask(Texture::createTexture("assets/textures/grassMask.png", 2));
	RenderTool::enableModelBlend(grassModel);
	RenderTool::setModelUniformMaterial(grassModel, grassMat);
	scene->addChild(grassModel);

    //2 实体平面
	auto planeGeo = Geometry::createPlane(5.0, 5.0);
	auto planeMat = PhongMaterial::createMaterial();
	planeMat->setDiffuse(Texture::createTexture("assets/textures/box.png", 0));
	planeMat->setSpecularMask(Texture::createTexture("assets/textures/sp_mask.png", 1));
	auto planeMesh = Mesh::createObj(planeGeo, planeMat);
	planeMesh->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	scene->addChild(planeMesh);

	//3 半透明平面
	auto planeGeoTrans = Geometry::createPlane(10.0, 10.0);
	auto planeMatTrans = PhongMaterial::createMaterial();
	planeMatTrans->setDiffuse(Texture::createTexture("assets/textures/wall.jpg", 0));
	planeMatTrans->setOpacity(0.4f);
	auto planeMeshTrans = Mesh::createObj(planeGeoTrans, planeMatTrans);
	planeMeshTrans->enableBlend();
	planeMeshTrans->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	planeMeshTrans->setPosition(glm::vec3(0.0f, 0.0f, -6.0f));
	scene->addChild(planeMeshTrans);

	////4 实体平面
	//auto planeGeo2 = Geometry::createPlane(10.0, 10.0);
	//auto planeMat2 = PhongMaterial::createMaterial();
	//planeMat2->setDiffuse(Texture::createTexture("assets/textures/goku.jpg", 0));
	//planeMat2->setOpacity(0.5f);
	//auto planeMesh2 = Mesh::createObj(planeGeo2, planeMat2);
	//planeMesh2->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	//planeMesh2->rotateY(45.0f);
	//planeMesh2->enableBlend();
	//planeMesh2->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//scene->addChild(planeMesh2);


	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setSpecularIntensity(1.0f);
	ambLight.setColor(glm::vec3(0.2f));
}

void BlendTestEX::render()
{
	this->doTransform();

	//设置当前帧绘制的时候，opengl的必要状态机参数
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	//开启测试、设置基本写入状态，打开模板测试写入
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);//保证了模板缓冲可以被清理

	//默认颜色混合
	glDisable(GL_BLEND);

	//清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//将scene当作根节点开iteratie渲染
	//RenderTool::objectRender(scene, this);
	RenderTool::objectSortedRender(scene, this);
}

void BlendTestEX::meshRendering(Object* object)
{
	Mesh* mesh = (Mesh*)object;
	Geometry* geometry = mesh->getGeometry();
	Material* material = mesh->getMaterial();

	//设置渲染状态
	mesh->applyState();

	//1 决定使用哪个Shader 
	Shader& shader = pickShader(material->mType);

	//2 更新shader的uniform
	shader.begin();

	switch (material->mType) {
	case MaterialType::PhongMaterial: {
		PhongMaterial* phongMat = (PhongMaterial*)material;

		//diffuse贴图帧更新
		//将纹理采样器与纹理单元进行挂钩
		shader.setInt("sampler", 0);
		//将纹理与纹理单元进行挂钩
		phongMat->bindDiffuse();

		//高光蒙版的帧更新
		shader.setInt("specularMaskSampler", 1);
		phongMat->bindSpecularMask();

		//opacityMask的帧更新
		shader.setInt("opacityMaskSampler", 2);
		phongMat->bindOpcityMask();

		//mvp
		shader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

		auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
		shader.setMatrix3x3("normalMatrix", normalMatrix);

		//光源参数的uniform更新
		//directionalLight 的更新
		shader.setVector3("directionalLight.color", dirLight.getColor());
		shader.setVector3("directionalLight.direction", dirLight.getDirection());
		shader.setFloat("directionalLight.specularIntensity", dirLight.getSpecularIntensity());

		shader.setFloat("shiness", phongMat->getShiness());

		shader.setVector3("ambientLight.color", ambLight.getColor());

		//相机信息更新
		shader.setVector3("cameraPosition", rCamera.mPosition);

		//透明度
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
	default:
		break;
	}

	//3 绑定vao
	glBindVertexArray(geometry->getVao());

	//4 执行绘制命令
	glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

	shader.end();
}


Shader& BlendTestEX::pickShader(MaterialType type)
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