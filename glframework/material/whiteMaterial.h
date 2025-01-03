#pragma once

#include "material.h"
#include "../texture.h"

class WhiteMaterial : public Material
{
public:
    static WhiteMaterial* createMaterial();

    WhiteMaterial();
    WhiteMaterial(const WhiteMaterial&) = default;
    WhiteMaterial& operator=(const WhiteMaterial&) = default;
    ~WhiteMaterial();
};