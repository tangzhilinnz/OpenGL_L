#pragma once

#include "openGLRenderer.h"
#include "../glframework/texture.h"
#include "../glframework/shader.h"
#include "../glframework/mesh.h"
#include "../glframework/light/ambientLight.h"
#include "../glframework/light/spotLight.h"
#include "../glframework/material/phongMaterial.h"
#include "../application/camera/camera.h"

#include <vector>

class SpotLightEX : public OpenGLRenderer
{
public:
    // Constructors
    SpotLightEX() = default;
    SpotLightEX(const SpotLightEX&) = default;
    SpotLightEX& operator=(const SpotLightEX&) = default;
    ~SpotLightEX();

    SpotLightEX(const Camera& _rCamera);

    // Methods
    void render() override;
    void prepareScene() override;
    void prepareShader() override;

    void prepareVAO() {}
    void prepareTexture() {}


    // Add a mesh to the renderer
    void addMesh(Mesh& mesh);

    inline Mesh& getMeshAt(size_t index)
    {
        assert(index < meshes.size());
        return meshes[index];
    }

private:
    //根据Material类型不同，挑选不同的shader
    Shader& pickShader(MaterialType type);

private:
    void doTransform();

    const Camera& rCamera;
    Shader mPhongShader;
    SpotLight spotLight;
    AmbientLight ambLight;

    std::vector<Mesh> meshes;

    // Members requiring the class to handle their memory management
    Geometry* geometry{ nullptr };
    Texture* diffuseTex{ nullptr };
    Texture* maskTex{ nullptr };
    PhongMaterial* material0{ nullptr };
};