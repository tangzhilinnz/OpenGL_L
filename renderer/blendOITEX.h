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

    Object* opaqueObjects{ nullptr };
    Object* transparentObjects{ nullptr };

    Object* scene{ nullptr }; //The Manager for Whole Scene Transformations 
    Geometry* screenDrawing{ nullptr };

    std::vector<Mesh*> opaqueMeshVec;
    std::vector<Mesh*> transparentMeshVec;

    // ---------------------------------------------------------------------------------------------------------------

    AttachmentGL* opaqueTexture{ nullptr };
    AttachmentGL* opaqueDepthTexture{ nullptr };

    AttachmentGL* accumTexture{ nullptr };
    AttachmentGL* revealTexture{ nullptr };

    FboGL* opaqueFBO{ nullptr };
    FboGL* transparentFBO{ nullptr };

    // ----------------------------------------------------------------------------------------------------------------

private:

    void phongMeshRender(Object* object, Shader& phongShader);
    void whiteMeshRender(Object* object, Shader& whiteShader);
    void depthMeshRender(Object* object, Shader& depthShader);

    void opaqueMeshRender(Object* object)
    {
        Mesh* mesh = (Mesh*)object;
        Material* material = mesh->getMaterial();
        if (material->mType == MaterialType::PhongMaterial)
        {
            this->phongMeshRender(mesh, mOpaquePhongShader);
        }
        else if (material->mType == MaterialType::WhiteMaterial)
        {
            this->whiteMeshRender(mesh, mOpaqueWhiteShader);
        }
        else if (material->mType == MaterialType::DepthMaterial)
        {
            this->depthMeshRender(mesh, mOpaqueDepthShader);
        }
    }

    void transparentMeshRender(Object* object)
    {
        Mesh* mesh = (Mesh*)object;
        Material* material = mesh->getMaterial();
        if (material->mType == MaterialType::PhongMaterial)
        {
            this->phongMeshRender(mesh, mTransparentPhongShader);
        }
        else if (material->mType == MaterialType::WhiteMaterial)
        {
            this->whiteMeshRender(mesh, mTransparentWhiteShader);
        }
        else if (material->mType == MaterialType::DepthMaterial)
        {
            this->depthMeshRender(mesh, mTransparentDepthShader);
        }
    }

    void compositeRender()
    {
        this->mScreenCompositeShader.begin();

        //先切换纹理单元，然后绑定texture对象
        this->mScreenCompositeShader.setInt("accum", 0);
        this->accumTexture->bindAttmTex(0);

        this->mScreenCompositeShader.setInt("reveal", 1);
        this->revealTexture->bindAttmTex(1);

        GL_CALL(glBindVertexArray(screenDrawing->getVao()));
        GL_CALL(glDrawElements(GL_TRIANGLES, screenDrawing->getIndicesCount(), GL_UNSIGNED_INT, 0));

        this->mScreenCompositeShader.end();
    }
    void displayRender()
    {
        this->mScreenShader.begin();

        this->mScreenShader.setInt("screenTexSampler", 0);
        this->opaqueTexture->bindAttmTex(0);

        GL_CALL(glBindVertexArray(screenDrawing->getVao()));
        GL_CALL(glDrawElements(GL_TRIANGLES, screenDrawing->getIndicesCount(), GL_UNSIGNED_INT, 0));

        this->mScreenShader.end();
    }
};