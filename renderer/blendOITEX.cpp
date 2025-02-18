#include "blendOITEX.h"

#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/depthMaterial.h"
#include "../glframework/material/whiteMaterial.h"
#include "../glframework/material/envMaterial.h"

#include <iostream>
#include <string>


BlendOITEX::BlendOITEX(const Camera& _rCamera)
	: rCamera(_rCamera)
{
	pCamera = &rCamera;
}

BlendOITEX::~BlendOITEX()
{
	printf("---- ~BlendTestEX ----\n");
	RenderTool::sceneClear();
}

void BlendOITEX::prepareShader()
{
	mOpaquePhongShader.initShader("assets/shaders/OIT/OpaquePhong.vert", "assets/shaders/OIT/OpaquePhong.frag");
	mTransparentPhongShader.initShader("assets/shaders/OIT/TransparentPhong.vert", "assets/shaders/OIT/TransparentPhong.frag");
	mScreenCompositeShader.initShader("assets/shaders/OIT/ScreenComposite.vert", "assets/shaders/OIT/ScreenComposite.frag");
	mScreenShader.initShader("assets/shaders/OIT/ScreenDisplay.vert", "assets/shaders/OIT/ScreenDisplay.frag");
	mTransparentWhiteShader.initShader("assets/shaders/OIT/TransparentWhite.vert", "assets/shaders/OIT/TransparentWhite.frag");
	mOpaqueWhiteShader.initShader("assets/shaders/OIT/OpaqueWhite.vert", "assets/shaders/OIT/OpaqueWhite.frag");
	mTransparentDepthShader.initShader("assets/shaders/OIT/TransparentDepth.vert", "assets/shaders/OIT/TransparentDepth.frag");
	mOpaqueDepthShader.initShader("assets/shaders/OIT/OpaqueDepth.vert", "assets/shaders/OIT/OpaqueDepth.frag");
}

void BlendOITEX::prepareScene()
{
	this->prepareShader();

	Screen::init();
	
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

	//opaqueDepthTexture->genBuffer(AttachmentType::DEPTH_ATTM, BufferType::TEXTURE_2D,
	//	800, 600, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);

	opaqueDepthTexture->genBuffer(AttachmentType::DEPTH_STENCIL_ATTM, BufferType::RENDER_BUFFER,
	    800, 600, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

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

	// =========================== special objects ============================
	////grass model
	//auto grassModel = AssimpLoader::load("assets/fbx/grass.fbx");
	//grassModel->setScale(glm::vec3(0.01f));
	//grassModel->setPosition(glm::vec3(0.0f, 4.8f, 0.0f));
	//auto grassMat = PhongMaterial::createMaterial();
	//grassMat->setDiffuse(Texture::createTexture("assets/textures/grassColor.jpg", 0));
	//grassMat->setOpcityMask(Texture::createTexture("assets/textures/grassMask.png", 2));
	//RenderTool::setMaterial(grassModel, grassMat);
	//RenderTool::enableBlend(grassModel);
	//RenderTool::disableDepthWrite(grassModel);
	
	// Cube Map
	const char* paths[] = {
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/back.jpg",
		"assets/textures/skybox/front.jpg"
	};
	Texture* cubeMapTex = Texture::createCubeMapTexture(paths, 0);

	// Sphere Map
	Texture* sphereMapTex = Texture::createTexture("assets/textures/bk.jpg", 0);
	sphereMapTex->setMinFilter(GL_NEAREST);

	// Left Cross Map
	Texture* leftCrossMapTex = Texture::createTexture("assets/textures/Daylight.png", 0);
	leftCrossMapTex->setMinFilter(GL_NEAREST);

	Skybox::init(SkyboxType::SPHERE_MAP, &rCamera);
	Skybox::setTexture(sphereMapTex);

	//Skybox::init(SkyboxType::CUBE_MAP, &rCamera);
	//Skybox::setTexture(cubeMapTex);

    //Skybox::init(SkyboxType::LEFT_CROSS_MAP, &rCamera);
	Skybox::resetType(SkyboxType::LEFT_CROSS_MAP);
	Skybox::setTexture(leftCrossMapTex);

	//Skybox::resetType(SkyboxType::CUBE_MAP);
	//Skybox::setTexture(cubeMapTex);

	// ========================================================================

	//1 背包模型
	auto backpack = AssimpLoader::load("assets/fbx/bag/backpack.obj");
	backpack->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	RenderTool::setOpcity(backpack, 0.5f);
	backpack->setScale(glm::vec3(1.6f));
	this->transMeshAttch(backpack);

	
	auto whiteMat = WhiteMaterial::createMaterial();
	whiteMat->setOpacity(0.3f);
	auto depthMat = DepthMaterial::createMaterial();

    //2 实体方盒
	auto boxGeo = Geometry::createBox(2.0f);
	auto boxMat = PhongMaterial::createMaterial();
	boxMat->setDiffuse(Texture::createTexture("assets/textures/box.png", 0));
	boxMat->setSpecularMask(Texture::createTexture("assets/textures/sp_mask.png", 1));
	auto boxMesh = Mesh::createObj(boxGeo, boxMat/*whiteMat*//*depthMat*/);
	boxMesh->enableStencilTest();
	boxMesh->stencilFunc(GL_ALWAYS, 1, 0xFF);
	boxMesh->stencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	boxMesh->stencilMask(0xFF);
	this->opaqueMeshAttch(boxMesh);

	auto meshBoxBound = Mesh::createObj(boxGeo, whiteMat);
	meshBoxBound->setPosition(boxMesh->getPosition());
	meshBoxBound->setScale(glm::vec3(1.3f));
	meshBoxBound->enableStencilTest();
	meshBoxBound->stencilFunc(GL_NOTEQUAL, 1, 0xFF);
	meshBoxBound->stencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	meshBoxBound->stencilMask(0x00);
	this->transMeshAttch(meshBoxBound);

	auto boxGeo2 = Geometry::createBox(1.4f);
	auto boxMat2 = PhongMaterial::createMaterial();
	boxMat2->setDiffuse(Texture::createTexture("assets/textures/box.png", 0));
	boxMat2->setSpecularMask(Texture::createTexture("assets/textures/sp_mask.png", 1));
	auto boxMesh2 = Mesh::createObj(boxGeo2, boxMat2/*whiteMat*//*depthMat*/);
	boxMesh2->enableStencilTest();
	boxMesh2->stencilFunc(GL_ALWAYS, 1, 0xFF);
	boxMesh2->stencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	boxMesh2->stencilMask(0xFF);
	boxMesh2->setPosition(glm::vec3(0.0f, 0.0f, -1.8f));
	this->opaqueMeshAttch(boxMesh2);

	auto meshBoxBound2 = Mesh::createObj(boxGeo2, whiteMat);
	meshBoxBound2->setPosition(boxMesh2->getPosition());
	meshBoxBound2->setScale(glm::vec3(1.4f));
	meshBoxBound2->enableStencilTest();
	meshBoxBound2->stencilFunc(GL_NOTEQUAL, 1, 0xFF);
	meshBoxBound2->stencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	meshBoxBound2->stencilMask(0x00);
	this->transMeshAttch(meshBoxBound2);

	//实体dog
	auto dog = AssimpLoader::load("assets/fbx/dog/spot.obj");
	dog->setPosition(glm::vec3(0.0f, -3.3f, 0.0f));
	dog->setScale(glm::vec3(1.5f));
	this->opaqueMeshAttch(dog);
	
	// transparent sphere
	auto sphereGeo = Geometry::createSphere(1.0f);
	auto sphereMat = PhongMaterial::createMaterial();
	sphereMat->setDiffuse(Texture::createTexture("assets/textures/earth.png", 0));
	sphereMat->setOpacity(0.4461f);
	auto sphereMesh = Mesh::createObj(sphereGeo, sphereMat/*whiteMat*//*depthMat*/);
	sphereMesh->setPosition(glm::vec3(0.0f, 3.4f, 0.0f));
	this->transMeshAttch(sphereMesh);

	//3 半透明平面
	auto planeGeoTrans = Geometry::createPlane(10.0f, 10.0f);
	auto planeMatTrans = PhongMaterial::createMaterial();
	planeMatTrans->setDiffuse(Texture::createTexture("assets/textures/wall.jpg", 0));
	planeMatTrans->setOpacity(0.82f);
	auto planeMeshTrans = Mesh::createObj(planeGeoTrans, planeMatTrans);
	planeMeshTrans->setPosition(glm::vec3(0.0f, 0.0f, -6.0f));
	this->transMeshAttch(planeMeshTrans);

	//透明窗口1
	auto windowGeo2 = Geometry::createPlane(6.0f, 6.0f);
	auto windowMat2 = PhongMaterial::createMaterial();
	windowMat2->setDiffuse(Texture::createTexture("assets/textures/window-b.png", 0));
	auto windowMesh2 = Mesh::createObj(windowGeo2, windowMat2);
	windowMesh2->setPosition(glm::vec3(0.0f, 0.1f, -3.6f));
	windowMesh2->rotateY(20.0f);
	this->transMeshAttch(windowMesh2);

	//4 实体平面
	auto planeGeo2 = Geometry::createPlane(10.0f, 10.0f);
	auto planeMat2 = PhongMaterial::createMaterial();
	planeMat2->setDiffuse(Texture::createTexture("assets/textures/window.png", 0));
	planeMat2->setOpacity(0.8f);
	auto planeMesh2 = Mesh::createObj(planeGeo2, planeMat2);
	planeMesh2->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	planeMesh2->rotateY(45.0f);
	this->transMeshAttch(planeMesh2);

	// Big terrain plane
	//auto planeGeoBig = Geometry::createPlane(1500.0f, 1500.0f, 200.f, 200.f);
	auto planeGeoBig = Geometry::createSimpleTerrain("assets/textures/HMtest.tga", 500.0f, 68.f, 0.f, 100.f, 100.f);
	auto planeMatBig = PhongMaterial::createMaterial();
	Texture* planTexBig = Texture::createTexture("assets/textures/grass.jpg"/*"assets/textures/HMtest16.tga"*/, 0);
	planTexBig->enableAnisotropicFilter(16.f);
	planeMatBig->setDiffuse(planTexBig);
	auto planeMeshBig = Mesh::createObj(planeGeoBig, planeMatBig);

	planeMeshBig->enableCullFace();     // Enable face culling
	planeMeshBig->cullFace(GL_BACK);    // Cull back faces
	planeMeshBig->setFrontFace(GL_CCW);  // Counter Clockwise is the front face

	planeMeshBig->setPosition(glm::vec3(0.0f, -10.0f, 0.0f));
	//planeMeshBig->rotateX(-90.0f);
	this->opaqueMeshAttch(planeMeshBig);

	//透明窗口2
	auto windowGeo = Geometry::createPlane(4.0f, 4.0f);
	auto windowMat = PhongMaterial::createMaterial();
	windowMat->setDiffuse(Texture::createTexture("assets/textures/window-g.png", 0));
	auto windowMesh = Mesh::createObj(windowGeo, windowMat);
	windowMesh->setPosition(glm::vec3(3.0f, 0.0f, 3.0f));
	this->transMeshAttch(windowMesh);

	//透明窗口3
	auto windowGeo3 = Geometry::createPlane(6.24f, 6.24f);
	auto windowMat3 = PhongMaterial::createMaterial();
	windowMat3->setDiffuse(Texture::createTexture("assets/textures/window-r.png", 0));
	auto windowMesh3 = Mesh::createObj(windowGeo3, windowMat3);
	windowMesh3->setPosition(glm::vec3(3.0f, 0.1f, -3.6f));
	windowMesh3->rotateY(86.0f);
	this->transMeshAttch(windowMesh3);

	dirLight.mDirection = glm::vec3(-1.0f);
	dirLight.setColor(glm::vec3(0.8f));
	dirLight.setSpecularIntensity(0.4f);
	ambLight.setColor(glm::vec3(0.68f));

	this->setOITState();
	this->separateMesh();
}

void BlendOITEX::render()
{
	//this->doTransform();

	glm::vec4 zeroFillerVec(0.0f);
	glm::vec4 oneFillerVec(1.0f);

	const size_t opaqueMeshNum = opaqueVec.size();
	const size_t transparentMeshNum = transVec.size();

	opaqueFBO->begin();
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

	Skybox::render();
	this->opaqueMeshRender();

	opaqueFBO->end();

	if (transparentMeshNum > 0)
	{
		transparentFBO->begin();
		glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
		this->transMeshRender();
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

void BlendOITEX::opaqueMeshAttch(Object* object)
{
	RenderTool::extractMesh(object, this->opaqueVec);
}

void BlendOITEX::transMeshAttch(Object* object)
{
	RenderTool::extractMesh(object, this->transVec);
}

void BlendOITEX::separateMesh()
{
	for (Mesh* mesh: this->opaqueVec)
	{
		Material* mat = mesh->getMaterial();

		switch (mat->mType)
		{
		case MaterialType::PhongMaterial:
			this->opaquePhongVec.push_back(mesh);
		    break;
		case MaterialType::WhiteMaterial:
			this->opaqueWhiteVec.push_back(mesh);
			break;
		case MaterialType::DepthMaterial:
			this->opaqueDepthVec.push_back(mesh);
			break;
		default:
			assert(-1);
			break;
		}
	}

	for (Mesh* mesh : this->transVec)
	{
		Material* mat = mesh->getMaterial();

		switch (mat->mType)
		{
		case MaterialType::PhongMaterial:
			this->transPhongVec.push_back(mesh);
			break;
		case MaterialType::WhiteMaterial:
			this->transWhiteVec.push_back(mesh);
			break;
		case MaterialType::DepthMaterial:
			this->transDepthVec.push_back(mesh);
			break;
		default:
			assert(-1);
			break;
		}
	}
}

void BlendOITEX::setOITState()
{
	for (Mesh* mesh : this->opaqueVec)
	{
		// 启用 depth test
		mesh->enableDepthTest();
		// depth test 通过 条件为 less
		mesh->depthFunc(GL_LESS);
		// 可以写入 depth buffer
		mesh->enableDepthWrite();
		// 禁止 blend
		mesh->disableBlend();
	}

	for (Mesh* mesh : this->transVec)
	{
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
}

void BlendOITEX::opaqueMeshRender()
{
	this->phongMeshRender(this->mOpaquePhongShader, this->opaquePhongVec);
	this->whiteMeshRender(this->mOpaqueWhiteShader, this->opaqueWhiteVec);
	this->depthMeshRender(this->mOpaqueDepthShader, this->opaqueDepthVec);
}

void BlendOITEX::transMeshRender()
{
	this->phongMeshRender(this->mTransparentPhongShader, this->transPhongVec);
	this->whiteMeshRender(this->mTransparentWhiteShader, this->transWhiteVec);
	this->depthMeshRender(this->mTransparentDepthShader, this->transDepthVec);
}

void BlendOITEX::phongMeshRender(Shader& phongShader, std::vector<Mesh*>& meshVec)
{
	phongShader.begin();

	phongShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
	phongShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

	for (Mesh* mesh : meshVec)
	{
		Geometry* geometry = mesh->getGeometry();
		PhongMaterial* phongMat = (PhongMaterial*)mesh->getMaterial();

		//设置渲染状态
		mesh->applyState();

		//diffuse贴图帧更新
	    //将纹理采样器与纹理单元进行挂钩
		phongShader.setInt("sampler", 0);
		//将纹理与纹理单元进行挂钩
		phongMat->bindDiffuse();

		//高光蒙版的帧更新
		phongShader.setInt("specularMaskSampler", 1);
		phongMat->bindSpecularMask();

		//opacityMask的帧更新
		phongShader.setInt("opacityMaskSampler", 2);
		phongMat->bindOpcityMask();

		//mvp
		phongShader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());

		auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
		phongShader.setMatrix3x3("normalMatrix", normalMatrix);

		//光源参数的uniform更新
		//directionalLight 的更新
		phongShader.setVector3("directionalLight.color", dirLight.getColor());
		phongShader.setVector3("directionalLight.direction", dirLight.getDirection());
		phongShader.setFloat("directionalLight.specularIntensity", dirLight.getSpecularIntensity());

		phongShader.setFloat("shiness", phongMat->getShiness());

		phongShader.setVector3("ambientLight.color", ambLight.getColor());

		//相机信息更新
		phongShader.setVector3("cameraPosition", rCamera.mPosition);

		//透明度
		phongShader.setFloat("opacity", phongMat->getOpacity());

		GL_CALL(glBindVertexArray(geometry->getVao()));

		//4 执行绘制命令
		GL_CALL(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
	}

	phongShader.end();
}

void BlendOITEX::whiteMeshRender(Shader& whiteShader, std::vector<Mesh*>& meshVec)
{
	whiteShader.begin();

	whiteShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
	whiteShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

	for (Mesh* mesh : meshVec)
	{
		Geometry* geometry = mesh->getGeometry();
		Material* mat = mesh->getMaterial();

		mesh->applyState();

		whiteShader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		whiteShader.setFloat("opacity", mat->getOpacity());

		GL_CALL(glBindVertexArray(geometry->getVao()));
		GL_CALL(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
	}

	whiteShader.end();
}

void BlendOITEX::depthMeshRender(Shader& depthShader, std::vector<Mesh*>& meshVec)
{
	depthShader.begin();

	depthShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
	depthShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

	for (Mesh* mesh : meshVec)
	{
		Geometry* geometry = mesh->getGeometry();
		Material* mat = mesh->getMaterial();

		mesh->applyState();

		depthShader.setMatrix4x4("modelMatrix", mesh->getModelMatrix());
		depthShader.setFloat("opacity", mat->getOpacity());

		depthShader.setFloat("near", rCamera.mNear);
		depthShader.setFloat("far", rCamera.mFar);

		GL_CALL(glBindVertexArray(geometry->getVao()));
		GL_CALL(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
	}

	depthShader.end();
}

void BlendOITEX::compositeRender()
{
	this->mScreenCompositeShader.begin();

	//先切换纹理单元，然后绑定texture对象
	this->mScreenCompositeShader.setInt("accum", 0);
	this->accumTexture->bindAttmTex(0);

	this->mScreenCompositeShader.setInt("reveal", 1);
	this->revealTexture->bindAttmTex(1);

	Screen::draw();

	this->mScreenCompositeShader.end();
}

void BlendOITEX::displayRender()
{
	this->mScreenShader.begin();

	this->mScreenShader.setInt("screenTexSampler", 0);
	this->opaqueTexture->bindAttmTex(0);

	Screen::draw();

	this->mScreenShader.end();
}