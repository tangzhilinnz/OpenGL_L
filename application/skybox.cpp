#include "Skybox.h"

void Skybox::render()
{
    Skybox& pMan = Skybox::privGetInstance();
    pMan.privRender();
}

void Skybox::init(SkyboxType type, const Camera* cam)
{
    Skybox& pMan = Skybox::privGetInstance();
    pMan.privInit(type, cam);
}

void Skybox::resetType(SkyboxType type)
{
    Skybox& pMan = Skybox::privGetInstance();
    pMan.privResetType(type);
}

void Skybox::setTexture(Texture* tex)
{
    Skybox& pMan = Skybox::privGetInstance();
    pMan.privSetTexture(tex);
}

Skybox::~Skybox()
{
    printf("---- ~Skybox ----\n");
}

//--------------------private impl methods --------------------
void Skybox::privSetTexture(Texture* tex)
{
    assert(this->pSkyboxMat);
    this->pSkyboxMat->setEnvTex(tex);
}

void Skybox::privResetType(SkyboxType type)
{
    this->mType = type;

    switch (this->mType)
    {
    case SkyboxType::CUBE_MAP:
    {
        this->pSkyboxGeo = Geometry::createBox(1.0f, false);
        this->pSkyboxSdr->initShader("assets/shaders/Skybox/CubeMap.vert",
            "assets/shaders/Skybox/CubeMap.frag");
    }
    break;
    case SkyboxType::SPHERE_MAP:
    {
        this->pSkyboxGeo = Geometry::createBox(1.0f, false);
        this->pSkyboxSdr->initShader("assets/shaders/Skybox/SphereMap.vert",
            "assets/shaders/Skybox/SphereMap.frag");
    }
    break;
    case SkyboxType::LEFT_CROSS_MAP:
    {
        this->pSkyboxGeo = Geometry::createBox(1.0f, true);
        this->pSkyboxSdr->initShader("assets/shaders/Skybox/LeftCrossMap.vert",
            "assets/shaders/Skybox/LeftCrossMap.frag");
    }
    break;
    default:
        // something is wrong
        assert(false);
        break;
    }

    assert(this->pSkyboxGeo);
}

void Skybox::privInit(SkyboxType type, const Camera* cam)
{
    assert(cam);
    this->pCam = cam;

    this->pSkyboxMat = EnvMaterial::createMaterial();
    this->pSkyboxSdr = Shader::createShader();

    assert(this->pSkyboxMat && this->pSkyboxSdr);

    this->mState.enableDepthTest();
    this->mState.depthFunc(GL_LEQUAL); // GL_LEQUAL to avoid Z-fighting GL_LEQUAL
    this->mState.enableCullFace();     // Enable face culling
    this->mState.cullFace(GL_BACK);    // Cull back faces
    this->mState.setFrontFace(GL_CW);  // Clockwise is the front face

    this->privResetType(type);
}

void Skybox::privRender()
{
    if (this->mType == SkyboxType::CUBE_MAP)
    {
        assert(this->pSkyboxMat->getEnvTex()->getTextureTarget() ==
            GL_TEXTURE_CUBE_MAP);
    }

    this->mState.applyState();

    this->pSkyboxSdr->begin();

    glm::mat4 viewMatrix = this->pCam->GetViewMatrix();
    viewMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    this->pSkyboxSdr->setMatrix4x4("viewMatrix", viewMatrix);
    this->pSkyboxSdr->setMatrix4x4("projectionMatrix",
        this->pCam->GetProjectionMatrix());

    this->pSkyboxSdr->setInt("mapSampler", 0);
    this->pSkyboxMat->bindEnvTex(0);

    glBindVertexArray(this->pSkyboxGeo->getVao());
    glDrawElements(GL_TRIANGLES, this->pSkyboxGeo->getIndicesCount(),
        GL_UNSIGNED_INT, 0);

    this->pSkyboxSdr->end();
}
