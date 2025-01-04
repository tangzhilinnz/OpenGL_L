#pragma once

#include "material.h"
#include "../texture.h"

class DepthMaterial : public Material
{
public:
    static DepthMaterial* createMaterial();

    DepthMaterial();
    DepthMaterial(const DepthMaterial&) = default;
    DepthMaterial& operator=(const DepthMaterial&) = default;
    ~DepthMaterial();
};