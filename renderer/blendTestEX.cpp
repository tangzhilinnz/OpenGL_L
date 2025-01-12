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
	
	// -------------------------------------------------------- need to refractor --------------------------------------------------------
	glGenFramebuffers(1, &this->opaqueFBO);
	glGenFramebuffers(1, &this->transparentFBO);

	glGenTextures(1, &this->opaqueTexture);            // 生成 texture (opaqueTexture)
	glBindTexture(GL_TEXTURE_2D, opaqueTexture); // 绑定 texture (opaqueTexture 绑定到 CL_TEXTURE_2D 上)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_HALF_FLOAT,
		NULL); // 设置 opaqueTexture 属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); // 解绑 opaqueTexture

	glGenTextures(1, &this->opaqueDepthTexture);            // 绑定 texture (opaqueDepthTexture)
	glBindTexture(GL_TEXTURE_2D, opaqueDepthTexture); // 设置 opaqueDepthTexture 属性
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 将 opaqueTexture, opaqueDepthTexture 绑定到 opaqueGBO 上
	// opaqueTexture 用于接收 GL_COLOR_ATTACHMENT0
	// opaqueDepthTexture 用于接收 GL_DEPTH_ATTACHMENT
	glBindFramebuffer(GL_FRAMEBUFFER, this->opaqueFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->opaqueTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->opaqueDepthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::FRAMEBUFFER:: Opaque framebuffer is not complete!" << std::endl;
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 解绑  opaqueFBO

	// 3.2 设置 transparentFBO 对应的 texture
	// 设置 transparentFBO 的渲染纹理
	// transparentShader 中的 accum -> accumTexture, reveal -> revealTexture
	glGenTextures(1, &this->accumTexture);
	glBindTexture(GL_TEXTURE_2D, accumTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &this->revealTexture);
	glBindTexture(GL_TEXTURE_2D, revealTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 800, 600, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->transparentFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->accumTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->revealTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->opaqueDepthTexture,
		0); // opaque framebuffer's depth texture

	glDrawBuffers(2, this->transparentDrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!" << std::endl;
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// -------------------------------------------------------- need to refractor --------------------------------------------------------

	scene = Object::createObj();
	transparentObjects = Object::createObj();
	opaqueObjects = Object::createObj();

	////1 背包模型
	auto backpack = AssimpLoader::load("assets/fbx/bag/backpack.obj");
	backpack->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	RenderTool::setModelOpcity(backpack, 0.6f);
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
	planeMatTrans->setOpacity(0.64f);
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

	RenderTool::extractMesh(transparentObjects, transparentMeshVec);
	RenderTool::extractMesh(opaqueObjects, opaqueMeshVec);

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
	auto geo = Geometry::createScreenPlane();
	//auto tex = Texture::createTexture("assets/textures/grassColor.jpg", 0);
	auto mat = ScreenMaterial::createMaterial(nullptr);
	this->toScreen = Mesh::createObj(geo, mat);


	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setSpecularIntensity(0.65f);
	ambLight.setColor(glm::vec3(0.5f));
}

void BlendTestEX::render()
{
	this->doTransform();

	//将scene当作根节点开iteratie渲染
	//RenderTool::objectRender(scene, this);
	//RenderTool::objectSortedRender(scene, this);

	glm::vec4 zeroFillerVec(0.0f);
	glm::vec4 oneFillerVec(1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, this->opaqueFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (size_t i = 0; i < this->opaqueMeshVec.size(); i++)
	{
		Mesh* mesh = opaqueMeshVec[i];
		this->meshRender(mesh, mOpaquePhongShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->transparentFBO);
	glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
	glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
	for (size_t i = 0; i < this->transparentMeshVec.size(); i++)
	{
		Mesh* mesh = transparentMeshVec[i];
		this->meshRender(mesh, mTransparentPhongShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 混合 screen model +　accumTexture + revealTexture + opaqueTexture -> compositeShader-> opaqueTexture
	glBindFramebuffer(GL_FRAMEBUFFER, this->opaqueFBO);// 绑定 opaqueFBO
	glDepthFunc(GL_ALWAYS);                            // 总是通过 depth test
	glEnable(GL_BLEND);                                // 启用 blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 混合权重设为 alpha_src, 1-alpha_src

	mScreenCompositeShader.begin();

		Geometry* geometry = toScreen->getGeometry();
		//先切换纹理单元，然后绑定texture对象
		glActiveTexture(GL_TEXTURE0 + 0);
		mScreenCompositeShader.setInt("accum", 0);
		glBindTexture(GL_TEXTURE_2D, this->accumTexture);

		glActiveTexture(GL_TEXTURE0 + 1);
		mScreenCompositeShader.setInt("reveal", 1);
		glBindTexture(GL_TEXTURE_2D, this->revealTexture);

		glBindVertexArray(geometry->getVao());
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

	mScreenCompositeShader.end();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	mScreenShader.begin();

		//Geometry* geometry = toScreen->getGeometry();
		//先切换纹理单元，然后绑定texture对象
		glActiveTexture(GL_TEXTURE0 + 0);
		mScreenShader.setInt("screenTexSampler", 0);
		glBindTexture(GL_TEXTURE_2D, this->opaqueTexture);

		glBindVertexArray(geometry->getVao());
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

	mScreenShader.end();

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