#pragma once

#include "material.h"
#include "../texture.h"

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    PhongMaterial(const PhongMaterial&) = default;
    PhongMaterial& operator=(const PhongMaterial&) = default;
    ~PhongMaterial();

    // Constructor accepting a unique_ptr for mDiffuse
    explicit PhongMaterial(Texture* diffuse, Texture* specularMask, float shiness = 1.0f);

    inline void setShiness(float shiness)
    { 
        mShiness = shiness;
    }
    inline void setDiffuse(Texture* diffuse)
    {
        mDiffuse = diffuse;
    }
    inline void setSpecularMask(Texture* specularMask)
    {
        mSpecularMask = specularMask;
    }
    inline void bindDiffuse() const
    {
        return mDiffuse->bind();
    }
    inline void bindSpecularMask() const
    {
        return mSpecularMask->bind();
    }
    inline float getShiness() const
    {
        return mShiness;
    }
    inline Texture* getDiffuseTex() const
    {
        return mDiffuse;
    }
    inline Texture* getSpecularMask() const
    {
        return mSpecularMask;
    }

private:
    // PhongMaterial utilizes Texture without taking responsibility for its
    // memory management or initialization
    Texture* mDiffuse{ nullptr };
    Texture* mSpecularMask{ nullptr };
    // Shininess of the material
    float mShiness{ 1.0f };
};