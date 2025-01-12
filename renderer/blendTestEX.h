#pragma once

#include "openGLRenderer.h"
#include "renderTool.h"
#include "../glframework/shader.h"
#include "../glframework/mesh.h"
#include "../glframework/scene.h"
#include "../glframework/light/ambientLight.h"
#include "../glframework/light/spotLight.h"
#include "../glframework/light/pointLight.h"
#include "../glframework/light/directionalLight.h"
#include "../application/camera/camera.h"
#include "../application/assimpLoader.h"

#include <vector>

class BlendTestEX : public OpenGLRenderer
{
public:
    // Constructors
    BlendTestEX() = default;
    BlendTestEX(const BlendTestEX&) = default;
    BlendTestEX& operator=(const BlendTestEX&) = default;
    ~BlendTestEX();

    BlendTestEX(const Camera& _rCamera);

    void doTransform() { scene->rotateY(glm::radians(10.0f)); };

    //void meshRendering(Object* object) override;

    // Methods
    void prepareScene() override;
    void prepareShader() override;
    void render() override;

    void prepareVAO() override {}
    void prepareTexture() override {}


    void meshRender(Object* object, Shader& shader);

//private:
//    //根据Material类型不同，挑选不同的shader
//    Shader& pickShader(MaterialType type);

private:
    const Camera& rCamera;
    Shader mOpaquePhongShader;
    Shader mTransparentPhongShader;
    Shader mScreenCompositeShader;
    Shader mScreenShader;
    Shader mWhiteShader;
    Shader mDepthShader;


    AmbientLight ambLight;
    DirectionalLight dirLight;

    Object* opaqueObjects{ nullptr };
    Object* transparentObjects{ nullptr };
    Mesh* toScreen{ nullptr };
    Object* scene{ nullptr };

    std::vector<Mesh*> opaqueMeshVec;
    std::vector<Mesh*> transparentMeshVec;

    // -------------------------------------------------------- need to refractor --------------------------------------------------------
    GLuint opaqueFBO{ 0 };
    GLuint transparentFBO{ 0 };
    GLuint opaqueTexture{ 0 };
    GLuint opaqueDepthTexture{ 0 };
    GLuint accumTexture{ 0 };
    GLuint revealTexture{ 0 };

    const GLenum transparentDrawBuffers[2] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    // -------------------------------------------------------- need to refractor --------------------------------------------------------
};