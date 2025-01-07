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

#include <vector>

class StencilTestEX : public OpenGLRenderer
{
public:
    // Constructors
    StencilTestEX() = default;
    StencilTestEX(const StencilTestEX&) = default;
    StencilTestEX& operator=(const StencilTestEX&) = default;
    ~StencilTestEX();

    StencilTestEX(const Camera& _rCamera);

    void doTransform() {};

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