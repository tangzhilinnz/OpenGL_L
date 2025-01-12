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

	//离屏渲染的box
	auto boxGeo = Geometry::createBox(5.0f);
	auto boxMat = PhongMaterial::createMaterial();
	boxMat->setDiffuse(Texture::createTexture("assets/textures/luffy.jpg", 0));
	auto boxMesh = Mesh::createObj(boxGeo, boxMat);
	sceneOff->addChild(boxMesh);

	//贴到屏幕上的矩形
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

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFBO()));

	//设置当前帧绘制的时候，opengl的必要状态机参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glDepthMask(GL_TRUE));

	GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
	GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));

	//开启测试、设置基本写入状态，打开模板测试写入
	GL_CALL(glEnable(GL_STENCIL_TEST));
	GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	GL_CALL(glStencilMask(0xFF));//保证了模板缓冲可以被清理

	//默认颜色混合
	GL_CALL(glDisable(GL_BLEND));

	//清理画布 
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	RenderTool::objectRender(sceneOff, [this](Object* obj){ this->meshRendering(obj); });

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//设置当前帧绘制的时候，opengl的必要状态机参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glDepthMask(GL_TRUE));

	GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
	GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));

	//开启测试、设置基本写入状态，打开模板测试写入
	GL_CALL(glEnable(GL_STENCIL_TEST));
	GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	GL_CALL(glStencilMask(0xFF));//保证了模板缓冲可以被清理

	//默认颜色混合
	GL_CALL(glDisable(GL_BLEND));

	//清理画布 
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	RenderTool::objectRender(scene, [this](Object* obj) { this->meshRendering(obj); });
}

void FboTestEX::meshRendering(Object* object)
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

	//3 绑定vao
	glBindVertexArray(geometry->getVao());

	//4 执行绘制命令
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