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

    void meshRendering(Object* object) override;

    // Methods
    void prepareScene() override;
    void prepareShader() override;
    void render() override;

    void prepareVAO() override {}
    void prepareTexture() override {}

private:
    //根据Material类型不同，挑选不同的shader
    Shader& pickShader(MaterialType type);

private:
    const Camera& rCamera;
    Shader mPhongShader;
    Shader mWhiteShader;
    Shader mDepthShader;
    AmbientLight ambLight;
    DirectionalLight dirLight;

    Object* scene{ nullptr };
};