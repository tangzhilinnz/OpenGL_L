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
#include "../application/skybox.h"

#include <vector>

class BlendOITEX : public OpenGLRenderer
{
public:
    // Constructors
    BlendOITEX() = default;
    BlendOITEX(const BlendOITEX&) = default;
    BlendOITEX& operator=(const BlendOITEX&) = default;
    ~BlendOITEX();

    BlendOITEX(const Camera& _rCamera);

    void doTransform() { scene->rotateY(glm::radians(16.0f)); };

    //void meshRendering(Object* object) override;

    // Methods
    void prepareScene() override;
    void prepareShader() override;
    void render() override;

    void prepareVAO() override {}
    void prepareTexture() override {}

private:
    const Camera& rCamera;
    Shader mOpaquePhongShader;
    Shader mTransparentPhongShader;
    Shader mScreenCompositeShader;
    Shader mScreenShader;
    Shader mTransparentWhiteShader;
    Shader mOpaqueWhiteShader;
    Shader mTransparentDepthShader;
    Shader mOpaqueDepthShader;

    AmbientLight ambLight;
    DirectionalLight dirLight;

    Object* scene{ nullptr }; //The Manager for Whole Scene Transformations 
    Geometry* screenDrawing{ Geometry::createScreenPlane() }; // The drawing Plane on screen

    // ---------------------------------------------------------------------------------------------------------------
    // framebuffer setting 
    AttachmentGL* opaqueTexture{ nullptr };
    AttachmentGL* opaqueDepthTexture{ nullptr };

    AttachmentGL* accumTexture{ nullptr };
    AttachmentGL* revealTexture{ nullptr };

    FboGL* opaqueFBO{ nullptr };
    FboGL* transparentFBO{ nullptr };

    // ----------------------------------------------------------------------------------------------------------------

private:
    std::vector<Mesh*> opaqueVec;
    std::vector<Mesh*> transVec;

    std::vector<Mesh*> opaqueDepthVec;
    std::vector<Mesh*> opaqueWhiteVec;
    std::vector<Mesh*> opaquePhongVec;

    std::vector<Mesh*> transDepthVec;
    std::vector<Mesh*> transWhiteVec;
    std::vector<Mesh*> transPhongVec;

public:
    void opaqueMeshAttch(Object* object);
    void transMeshAttch(Object* object);

    void opaqueMeshRender();
    void transMeshRender();
private:
    void phongMeshRender(Shader& phongShader, std::vector<Mesh*>& meshVec);
    void whiteMeshRender(Shader& whiteShader, std::vector<Mesh*>& meshVec);
    void depthMeshRender(Shader& depthShader, std::vector<Mesh*>& meshVec);
    void compositeRender();
    void displayRender();

    void separateMesh();
    void setOITState();
};