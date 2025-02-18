#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/shader.h"
#include "../glframework/mesh.h"
#include "../glframework/geometry.h"
#include "../glframework/material/envMaterial.h"
#include "../application/camera/camera.h"

enum class SkyboxType
{
    CUBE_MAP,
    SPHERE_MAP,
    LEFT_CROSS_MAP
};

class Skybox
{
private:
    SkyboxType mType = SkyboxType::CUBE_MAP;
    State mState;

    Geometry* pSkyboxGeo = nullptr;
    EnvMaterial* pSkyboxMat = nullptr;
    Shader* pSkyboxSdr = nullptr;
    const Camera* pCam = nullptr;

public:
    Skybox(const Skybox&) = delete;
    Skybox(Skybox&&) = delete;
    Skybox& operator=(const Skybox&) & = delete;
    Skybox& operator=(Skybox&&) & = delete;
    ~Skybox();

    static void render();
    static void setTexture(Texture* tex);
    static void init(SkyboxType type, const Camera* cam);
    static void resetType(SkyboxType type);

private:
    static Skybox& privGetInstance()
    {
        static Skybox mInstance;
        return mInstance;
    }

    void privRender();
    void privSetTexture(Texture* tex);
    void privInit(SkyboxType type, const Camera* cam);
    void privResetType(SkyboxType type);

private:
    Skybox() = default;
};

#endif // _SKYBOX_H_