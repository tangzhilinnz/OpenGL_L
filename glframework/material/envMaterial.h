#pragma once

#include "material.h"
#include "../texture.h"

class EnvMaterial : public Material
{
public:
    static EnvMaterial* createMaterial();
    static EnvMaterial* createMaterial(Texture* envTex);

    EnvMaterial();
    EnvMaterial(const EnvMaterial&) = default;
    EnvMaterial& operator=(const EnvMaterial&) = default;
    ~EnvMaterial();

    explicit EnvMaterial(Texture* envTex);

    void setEnvTex(Texture* envTex)
    {
        mEnvTex = envTex;
    }

    void bindEnvTex() const
    {
        return mEnvTex->bind();
    }

    void bindEnvTex(unsigned int mUnit) const
    {
        mEnvTex->setUnit(mUnit);
        return mEnvTex->bind();
    }

    inline Texture* getEnvTex() const
    {
        return mEnvTex;
    }

private:
    Texture* mEnvTex{ nullptr };
};