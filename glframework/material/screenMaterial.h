#pragma once

#include "material.h"
#include "../texture.h"

class ScreenMaterial : public Material
{
public:
    static ScreenMaterial* createMaterial();
    static ScreenMaterial* createMaterial(Texture* screenTexture);

    ScreenMaterial();
    ScreenMaterial(Texture* screenTexture);
    ScreenMaterial(const ScreenMaterial&) = default;
    ScreenMaterial& operator=(const ScreenMaterial&) = default;
    ~ScreenMaterial();

public:

     void bindScreenTex() const
    {
        mScreenTexture->bind();
    }

private:
    Texture* mScreenTexture{ nullptr };
};