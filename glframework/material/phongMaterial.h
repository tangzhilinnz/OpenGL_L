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

    void setShiness(float shiness)
    { 
        mShiness = shiness;
    }
    void setDiffuse(Texture* diffuse)
    {
        mDiffuse = diffuse;
    }
    void setSpecularMask(Texture* specularMask)
    {
        mSpecularMask = specularMask;
    }
    void setOpcityMask(Texture* opcityMask)
    {
        mOpcityMask = opcityMask;
    }
    void bindDiffuse() const
    {
        return mDiffuse->bind();
    }
    void bindSpecularMask() const
    {
        return mSpecularMask->bind();
    }
    void bindOpcityMask() const
    {
        return mOpcityMask->bind();
    }

    void bindDiffuse(unsigned int mUnit) const
    {
        mDiffuse->setUnit(mUnit);
        return mDiffuse->bind();
    }
    void bindSpecularMask(unsigned int mUnit) const
    {
        mSpecularMask->setUnit(mUnit);
        return mSpecularMask->bind();
    }
    void bindOpcityMask(unsigned int mUnit) const
    {
        mOpcityMask->setUnit(mUnit);
        return mOpcityMask->bind();
    }

    float getShiness() const
    {
        return mShiness;
    }
    Texture* getDiffuseTex() const
    {
        return mDiffuse;
    }
    Texture* getSpecularMask() const
    {
        return mSpecularMask;
    }
    Texture* getOpcityMask() const
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