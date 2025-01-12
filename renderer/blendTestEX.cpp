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

	glGenTextures(1, &this->opaqueTexture);            // ���� texture (opaqueTexture)
	glBindTexture(GL_TEXTURE_2D, opaqueTexture); // �� texture (opaqueTexture �󶨵� CL_TEXTURE_2D ��)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_HALF_FLOAT,
		NULL); // ���� opaqueTexture ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); // ��� opaqueTexture

	glGenTextures(1, &this->opaqueDepthTexture);            // �� texture (opaqueDepthTexture)
	glBindTexture(GL_TEXTURE_2D, opaqueDepthTexture); // ���� opaqueDepthTexture ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// �� opaqueTexture, opaqueDepthTexture �󶨵� opaqueGBO ��
	// opaqueTexture ���ڽ��� GL_COLOR_ATTACHMENT0
	// opaqueDepthTexture ���ڽ��� GL_DEPTH_ATTACHMENT
	glBindFramebuffer(GL_FRAMEBUFFER, this->opaqueFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->opaqueTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->opaqueDepthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::FRAMEBUFFER:: Opaque framebuffer is not complete!" << std::endl;
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // ���  opaqueFBO

	// 3.2 ���� transparentFBO ��Ӧ�� texture
	// ���� transparentFBO ����Ⱦ����
	// transparentShader �е� accum -> accumTexture, reveal -> revealTexture
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

	////1 ����ģ��
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

    //2 ʵ�巽��
	auto planeGeo = Geometry::createBox(3.0f);
	auto planeMat = PhongMaterial::createMaterial();
	planeMat->setDiffuse(Texture::createTexture("assets/textures/box.png", 0));
	planeMat->setSpecularMask(Texture::createTexture("assets/textures/sp_mask.png", 1));
	auto planeMesh = Mesh::createObj(planeGeo, planeMat);
	//planeMesh->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	opaqueObjects->addChild(planeMesh);

	//ʵ��dog
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

	//3 ��͸��ƽ��
	auto planeGeoTrans = Geometry::createPlane(10.0f, 10.0f);
	auto planeMatTrans = PhongMaterial::createMaterial();
	planeMatTrans->setDiffuse(Texture::createTexture("assets/textures/wall.jpg", 0));
	planeMatTrans->setOpacity(0.64f);
	auto planeMeshTrans = Mesh::createObj(planeGeoTrans, planeMatTrans);
	planeMeshTrans->setPosition(glm::vec3(0.0f, 0.0f, -6.0f));
	transparentObjects->addChild(planeMeshTrans);

	//͸������1
	auto windowGeo2 = Geometry::createPlane(6.0f, 6.0f);
	auto windowMat2 = PhongMaterial::createMaterial();
	windowMat2->setDiffuse(Texture::createTexture("assets/textures/window-b.png", 0));
	auto windowMesh2 = Mesh::createObj(windowGeo2, windowMat2);
	windowMesh2->setPosition(glm::vec3(0.0f, 0.1f, -3.6f));
	windowMesh2->rotateY(20.0f);
	transparentObjects->addChild(windowMesh2);

	//4 ʵ��ƽ��
	auto planeGeo2 = Geometry::createPlane(10.0f, 10.0f);
	auto planeMat2 = PhongMaterial::createMaterial();
	planeMat2->setDiffuse(Texture::createTexture("assets/textures/luffy.jpg", 0));
	planeMat2->setOpacity(0.72f);
	auto planeMesh2 = Mesh::createObj(planeGeo2, planeMat2);
	planeMesh2->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	planeMesh2->rotateY(45.0f);
	transparentObjects->addChild(planeMesh2);

	//͸������2
	auto windowGeo = Geometry::createPlane(4.0f, 4.0f);
	auto windowMat = PhongMaterial::createMaterial();
	windowMat->setDiffuse(Texture::createTexture("assets/textures/window-g.png", 0));
	auto windowMesh = Mesh::createObj(windowGeo, windowMat);
	windowMesh->setPosition(glm::vec3(3.0f, 0.0f, 3.0f));
	transparentObjects->addChild(windowMesh);


	//͸������3
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
		//��ֹд�� depth buffer
		mesh->disableDepthWrite();
		//���� blend
		mesh->enableBlend();
		//���� color_attachment0 �Ļ��Ȩ��Ϊ 1,1, d_accum = C_s + C_d_accum
		mesh->blendFunci(0, GL_ONE, GL_ONE);
		//���� color_attachment1 �Ļ��Ȩ��Ϊ 0, 1-C_s, d_reveal = C_d_reveal * (1-C_s_color)
		mesh->blendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		//���� blend ����Ϊ ADD ����, ʵ���ϲ�����Ҳ���ԣ���ΪĬ�Ͼ��� ADD ����
		mesh->blendEquation(GL_FUNC_ADD);
	}

	//������Ļ�ϵľ���
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

	//��scene�������ڵ㿪iteratie��Ⱦ
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


	// ��� screen model +��accumTexture + revealTexture + opaqueTexture -> compositeShader-> opaqueTexture
	glBindFramebuffer(GL_FRAMEBUFFER, this->opaqueFBO);// �� opaqueFBO
	glDepthFunc(GL_ALWAYS);                            // ����ͨ�� depth test
	glEnable(GL_BLEND);                                // ���� blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ���Ȩ����Ϊ alpha_src, 1-alpha_src

	mScreenCompositeShader.begin();

		Geometry* geometry = toScreen->getGeometry();
		//���л�����Ԫ��Ȼ���texture����
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
		//���л�����Ԫ��Ȼ���texture����
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

	//������Ⱦ״̬
	mesh->applyState();

	shader.begin();

		//diffuse��ͼ֡����
		//�����������������Ԫ���йҹ�
		shader.setInt("sampler", 0);
		//������������Ԫ���йҹ�
		phongMat->bindDiffuse();

		//�߹��ɰ��֡����
		shader.setInt("specularMaskSampler", 1);
		phongMat->bindSpecularMask();

		//opacityMask��֡����
		shader.setInt("opacityMaskSampler", 2);
		phongMat->bindOpcityMask();

		//mvp
		shader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		shader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
		shader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

		auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
		shader.setMatrix3x3("normalMatrix", normalMatrix);

		//��Դ������uniform����
		//directionalLight �ĸ���
		shader.setVector3("directionalLight.color", dirLight.getColor());
		shader.setVector3("directionalLight.direction", dirLight.getDirection());
		shader.setFloat("directionalLight.specularIntensity", dirLight.getSpecularIntensity());

		shader.setFloat("shiness", phongMat->getShiness());

		shader.setVector3("ambientLight.color", ambLight.getColor());

		//�����Ϣ����
		shader.setVector3("cameraPosition", rCamera.mPosition);

		//͸����
		shader.setFloat("opacity", material->getOpacity());

		glBindVertexArray(geometry->getVao());

		//4 ִ�л�������
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

	shader.end();
}