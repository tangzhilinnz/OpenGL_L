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

class DepthTest2EX : public OpenGLRenderer
{
public:
    // Constructors
    DepthTest2EX() = default;
    DepthTest2EX(const DepthTest2EX&) = default;
    DepthTest2EX& operator=(const DepthTest2EX&) = default;
    ~DepthTest2EX();

    DepthTest2EX(const Camera& _rCamera);

    void doTransform() {};

    void meshRendering(Object* object) override;

    // Methods
    void prepareScene() override;
    void prepareShader() override;
    void render() override;

    void prepareVAO() override {}
    void prepareTexture() override {}

private:
    //����Material���Ͳ�ͬ����ѡ��ͬ��shader
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