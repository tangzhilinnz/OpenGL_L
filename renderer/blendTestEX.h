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

    void meshRender(Object* object, Shader& shader);

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