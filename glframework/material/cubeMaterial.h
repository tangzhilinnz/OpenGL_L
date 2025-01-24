#pragma once

#include "material.h"
#include "../texture.h"

class CubeMaterial : public Material
{
public:
    static CubeMaterial* createMaterial();
    static CubeMaterial* createMaterial(Texture* cubeMap);

    CubeMaterial();
    CubeMaterial(const CubeMaterial&) = default;
    CubeMaterial& operator=(const CubeMaterial&) = default;
    ~CubeMaterial();

    explicit CubeMaterial(Texture* cubeMap);

    void setCubeMap(Texture* cubeMap)
    {
        mCubeMap = cubeMap;
    }

    void bindCubeMap() const
    {
        return mCubeMap->bind();
    }

    void bindCubeMap(unsigned int mUnit) const
    {
        mCubeMap->setUnit(mUnit);
        return mCubeMap->bind();
    }

    inline Texture* getCubeMap() const
    {
        return mCubeMap;
    }

private:
    Texture* mCubeMap{ nullptr };
};