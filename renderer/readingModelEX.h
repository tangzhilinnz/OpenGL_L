#pragma once

#include "sceneRenderer.h"
#include "../glframework/shader.h"
#include "../glframework/mesh.h"
#include "../glframework/scene.h"
#include "../glframework/light/ambientLight.h"
#include "../glframework/light/spotLight.h"
#include "../glframework/light/pointLight.h"
#include "../glframework/light/directionalLight.h"
#include "../application/camera/camera.h"

#include <vector>

class ReadingModelEX : public SceneRenderer
{
public:
    // Constructors
    ReadingModelEX() = default;
    ReadingModelEX(const ReadingModelEX&) = default;
    ReadingModelEX& operator=(const ReadingModelEX&) = default;
    ~ReadingModelEX();

    ReadingModelEX(const Camera& _rCamera);

    // Methods
    void prepareScene() override;
    void prepareShader() override;
    void meshRender(Object* object) override;

    void prepareVAO() {}
    void prepareTexture() {}

private:
    //根据Material类型不同，挑选不同的shader
    Shader& pickShader(MaterialType type);

private:
    const Camera& rCamera;
    Shader mPhongShader;
    Shader mWhiteShader;
    AmbientLight ambLight;
    DirectionalLight dirLight;
};