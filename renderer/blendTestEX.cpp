#include "blendTestEX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/depthMaterial.h"
#include "../glframework/material/whiteMaterial.h"
#include "../glframework/material/screenMaterial.h"

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
	//mPhongShader.initShader("assets/shaders/PhongOpcityMask.vert", "assets/shaders/PhongOpcityMask.frag");

	mOpaquePhongShader.initShader("assets/shaders/OIT/OpaquePhong.vert", "assets/shaders/OIT/OpaquePhong.frag");
	mTransparentPhongShader.initShader("assets/shaders/OIT/TransparentPhong.vert", "assets/shaders/OIT/TransparentPhong.frag");
	mScreenCompositeShader.initShader("assets/shaders/OIT/ScreenComposite.vert", "assets/shaders/OIT/ScreenComposite.frag");
	mScreenShader.initShader("assets/shaders/OIT/Screen.vert", "assets/shaders/OIT/Screen.frag");

	mWhiteShader.initShader("assets/shaders/White.vert", "assets/shaders/White.frag");
	mDepthShader.initShader("assets/shaders/Depth.vert", "assets/shaders/Depth.frag");
}

void BlendTestEX::prepareScene()
{
	this->prepareShader();
	
	opaqueFBO = FboGL::create();
	transparentFBO = FboGL::create();
	opaqueFBO->genFBO();
	transparentFBO->genFBO();

	opaqueTexture = AttachmentGL::create();
	opaqueDepthTexture = AttachmentGL::create();
	accumTexture = AttachmentGL::create();
	revealTexture = AttachmentGL::create();

	opaqueTexture->genBuffer(AttachmentType::COLOR_ATTM, BufferType::TEXTURE_2D, 800, 600,
		GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);

	opaqueDepthTexture->genBuffer(AttachmentType::DEPTH_ATTM, BufferType::TEXTURE_2D,
		800, 600, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

	accumTexture->genBuffer(AttachmentType::COLOR_ATTM, BufferType::TEXTURE_2D, 800, 600,
		GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);

	revealTexture->genBuffer(AttachmentType::COLOR_ATTM, BufferType::TEXTURE_2D, 800, 600,
		GL_R8, GL_RED, GL_FLOAT);

	opaqueFBO->bindAttm(opaqueTexture);
	opaqueFBO->bindAttm(opaqueDepthTexture);
	transparentFBO->bindAttm(accumTexture);
	transparentFBO->bindAttm(revealTexture);
	transparentFBO->bindAttm(opaqueDepthTexture);


	scene = Object::createObj();
	transparentObjects = Object::createObj();
	opaqueObjects = Object::createObj();

	////1 背包模型
	auto backpack = AssimpLoader::load("assets/fbx/bag/backpack.obj");
	backpack->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	RenderTool::setModelOpcity(backpack, 0.6f);
	backpack->setScale(glm::vec3(1.6f));
	transparentObjects->addChild(backpack);

	//grass model
	//auto grassGeo = Geometry::createPlane(5.0f, 5.0f);
	//auto grassMat = PhongMaterial::createMaterial();
	//grassMat->setDiffuse(Texture::createTexture("assets/textures/grassColor.jpg", 0));
	//grassMat->setOpcityMask(Texture::createTexture("assets/textures/grassMask.png", 2));
	//auto grassMesh = Mesh::createObj(grassGeo, grassMat);
	//grassMesh->enableBlend();
	//grassMesh->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//scene->addChild(grassMesh);
	//
	//auto grassModel = AssimpLoader::load("assets/fbx/grass.fbx");
	//grassModel->setScale(glm::vec3(0.02f));
	//auto grassMat = PhongMaterial::createMaterial();
	//grassMat->setDiffuse(Texture::createTexture("assets/textures/grassColor.jpg", 0));
	//grassMat->setOpcityMask(Texture::createTexture("assets/textures/grassMask.png", 2));
	//RenderTool::enableModelBlend(grassModel);
	//RenderTool::setModelUniformMaterial(grassModel, grassMat);
	//scene->addChild(grassModel);

    //2 实体方盒
	auto planeGeo = Geometry::createBox(3.0f);
	auto planeMat = PhongMaterial::createMaterial();
	planeMat->setDiffuse(Texture::createTexture("assets/textures/box.png", 0));
	planeMat->setSpecularMask(Texture::createTexture("assets/textures/sp_mask.png", 1));
	auto planeMesh = Mesh::createObj(planeGeo, planeMat);
	//planeMesh->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	opaqueObjects->addChild(planeMesh);

	//实体dog
	auto dog = AssimpLoader::load("assets/fbx/dog/spot.obj");
	dog->setPosition(glm::vec3(0.0f, -3.3f, 0.0f));
	dog->setScale(glm::vec3(1.5f));
	opaqueObjects->addChild(dog);
	
	// transparent sphere
	auto sphereGeo = Geometry::createSphere(1.0f);
	auto sphereMat = PhongMaterial::createMaterial();
	sphereMat->setDiffuse(Texture::createTexture("assets/textures/earth.png", 0));
	sphereMat->setOpacity(0.4461f);
	auto sphereMesh = Mesh::createObj(sphereGeo, sphereMat);
	sphereMesh->setPosition(glm::vec3(0.0f, 3.4f, 0.0f));
	transparentObjects->addChild(sphereMesh);

	//3 半透明平面
	auto planeGeoTrans = Geometry::createPlane(10.0f, 10.0f);
	auto planeMatTrans = PhongMaterial::createMaterial();
	planeMatTrans->setDiffuse(Texture::createTexture("assets/textures/wall.jpg", 0));
	planeMatTrans->setOpacity(0.82f);
	auto planeMeshTrans = Mesh::createObj(planeGeoTrans, planeMatTrans);
	planeMeshTrans->setPosition(glm::vec3(0.0f, 0.0f, -6.0f));
	transparentObjects->addChild(planeMeshTrans);

	//透明窗口1
	auto windowGeo2 = Geometry::createPlane(6.0f, 6.0f);
	auto windowMat2 = PhongMaterial::createMaterial();
	windowMat2->setDiffuse(Texture::createTexture("assets/textures/window-b.png", 0));
	auto windowMesh2 = Mesh::createObj(windowGeo2, windowMat2);
	windowMesh2->setPosition(glm::vec3(0.0f, 0.1f, -3.6f));
	windowMesh2->rotateY(20.0f);
	transparentObjects->addChild(windowMesh2);

	//4 实体平面
	auto planeGeo2 = Geometry::createPlane(10.0f, 10.0f);
	auto planeMat2 = PhongMaterial::createMaterial();
	planeMat2->setDiffuse(Texture::createTexture("assets/textures/luffy.jpg", 0));
	planeMat2->setOpacity(0.72f);
	auto planeMesh2 = Mesh::createObj(planeGeo2, planeMat2);
	planeMesh2->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	planeMesh2->rotateY(45.0f);
	transparentObjects->addChild(planeMesh2);

	//透明窗口2
	auto windowGeo = Geometry::createPlane(4.0f, 4.0f);
	auto windowMat = PhongMaterial::createMaterial();
	windowMat->setDiffuse(Texture::createTexture("assets/textures/window-g.png", 0));
	auto windowMesh = Mesh::createObj(windowGeo, windowMat);
	windowMesh->setPosition(glm::vec3(3.0f, 0.0f, 3.0f));
	transparentObjects->addChild(windowMesh);


	//透明窗口3
	auto windowGeo3 = Geometry::createPlane(6.24f, 6.24f);
	auto windowMat3 = PhongMaterial::createMaterial();
	windowMat3->setDiffuse(Texture::createTexture("assets/textures/window-r.png", 0));
	auto windowMesh3 = Mesh::createObj(windowGeo3, windowMat3);
	windowMesh3->setPosition(glm::vec3(3.0f, 0.1f, -3.6f));
	windowMesh3->rotateY(86.0f);
	transparentObjects->addChild(windowMesh3);

	scene->addChild(transparentObjects);
	scene->addChild(opaqueObjects);

	transparentMeshVec = RenderTool::extractMesh(transparentObjects);
	opaqueMeshVec = RenderTool::extractMesh(opaqueObjects);

	//transparentMeshVec.clear();
	//opaqueMeshVec.clear();

	for (size_t i = 0; i < opaqueMeshVec.size(); i++)
	{
		Mesh* mesh = opaqueMeshVec[i];
		// 启用 depth test
		mesh->enableDepthTest();
		// depth test 通过 条件为 less
		mesh->depthFunc(GL_LESS);
		// 可以写入 depth buffer
		mesh->enableDepthWrite();
		// 禁止 blend
		mesh->disableBlend();
	}

	for (size_t i = 0; i < transparentMeshVec.size(); i++)
	{
		Mesh* mesh = transparentMeshVec[i];
		//禁止写入 depth buffer
		mesh->disableDepthWrite();
		//启用 blend
		mesh->enableBlend();
		//设置 color_attachment0 的混合权重为 1,1, d_accum = C_s + C_d_accum
		mesh->blendFunci(0, GL_ONE, GL_ONE);
		//设置 color_attachment1 的混合权重为 0, 1-C_s, d_reveal = C_d_reveal * (1-C_s_color)
		mesh->blendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		//设置 blend 运算为 ADD 运算, 实际上不设置也可以，因为默认就是 ADD 运算
		mesh->blendEquation(GL_FUNC_ADD);
	}

	//贴到屏幕上的矩形
	this->screenDrawing = Geometry::createScreenPlane();

	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setColor(glm::vec3(0.4f));
	dirLight.setSpecularIntensity(0.3f);
	ambLight.setColor(glm::vec3(0.68f));
}

void BlendTestEX::render()
{
	this->doTransform();

	glm::vec4 zeroFillerVec(0.0f);
	glm::vec4 oneFillerVec(1.0f);

	const size_t opaqueMeshNum = opaqueMeshVec.size();
	const size_t transparentMeshNum = transparentMeshVec.size();

	opaqueFBO->begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (size_t i = 0; i < opaqueMeshNum; i++)
	{
		Mesh* mesh = opaqueMeshVec[i];
		this->meshRender(mesh, mOpaquePhongShader);
	}
	opaqueFBO->end();


	if (transparentMeshNum > 0)
	{
		transparentFBO->begin();
		glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
		for (size_t i = 0; i < transparentMeshNum; i++)
		{
			Mesh* mesh = transparentMeshVec[i];
			this->meshRender(mesh, mTransparentPhongShader);
		}
		transparentFBO->end();

		// 混合 accumTexture + revealTexture + opaqueTexture -> opaqueTexture
		opaqueFBO->begin();
		glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		this->compositeRender();
		opaqueFBO->end();
	}

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	this->displayRender();
}

void BlendTestEX::meshRender(Object* object, Shader& shader)
{
	Mesh* mesh = (Mesh*)object;
	Geometry* geometry = mesh->getGeometry();
	Material* material = mesh->getMaterial();
	PhongMaterial* phongMat = (PhongMaterial*)material;

	//设置渲染状态
	mesh->applyState();

	shader.begin();

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

		glBindVertexArray(geometry->getVao());

		//4 执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

	shader.end();
}