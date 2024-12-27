#pragma once

#include "material.h"
#include "../texture.h"

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    PhongMaterial(const PhongMaterial&) = default;
    PhongMaterial& operator=(const PhongMaterial&) = default;
    ~PhongMaterial() = default;

    // Constructor accepting a unique_ptr for mDiffuse
    explicit PhongMaterial(Texture* diffuse, float shiness = 1.0f);

    inline void setShiness(float shiness)
    { 
        mShiness = shiness;
    }
    inline void setDiffuse(Texture* diffuse)
    {
        mDiffuse = diffuse;
    }
    inline void bind() const
    {
        return mDiffuse->bind();
    }

    inline float getShiness() const
    {
        return mShiness;
    }

private:
    // PhongMaterial utilizes Texture without taking responsibility for its
    // memory management or initialization
    Texture* mDiffuse{ nullptr };
    // Shininess of the material
    float mShiness{ 1.0f };
};