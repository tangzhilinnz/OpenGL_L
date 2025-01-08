#pragma once

#include "material.h"
#include "../texture.h"

class PhongMaterial : public Material
{
public:
    static PhongMaterial* createMaterial();
    static PhongMaterial* createMaterial(Texture* diffuse, Texture* specularMask, Texture* opcityMask = nullptr, float shiness = 1.0f);

    PhongMaterial();
    PhongMaterial(const PhongMaterial&) = default;
    PhongMaterial& operator=(const PhongMaterial&) = default;
    ~PhongMaterial();

    // Constructor accepting a unique_ptr for mDiffuse
    explicit PhongMaterial(Texture* diffuse, Texture* specularMask, Texture* opcityMask, float shiness);

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
    inline void setOpcityMask(Texture* opcityMask)
    {
        mOpcityMask = opcityMask;
    }
    inline void bindDiffuse() const
    {
        return mDiffuse->bind();
    }
    inline void bindSpecularMask() const
    {
        return mSpecularMask->bind();
    }
    inline void bindOpcityMask() const
    {
        return mOpcityMask->bind();
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
    inline Texture* getOpcityMask() const
    {
        return mOpcityMask;
    }

private:
    // PhongMaterial utilizes Texture without taking responsibility for its
    // memory management or initialization
    Texture* mDiffuse{ nullptr };
    Texture* mSpecularMask{ nullptr };
    Texture* mOpcityMask{ nullptr };
    // Shininess of the material
    float mShiness{ 1.0f };
};