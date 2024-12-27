#pragma once

#include "../../glframework/core.h"

// Abstract Base Class
class OpenGLRenderer
{
public:
    // Pure virtual functions
    virtual void prepareVAO() = 0;
    virtual void prepareShader() = 0;
    virtual void prepareTexture() = 0;
    virtual void render() = 0;

    virtual void prepareScene()
    {
        prepareVAO();
        prepareShader();
        prepareTexture();
    }

    // Virtual destructor for proper cleanup in derived classes
    virtual ~OpenGLRenderer() = default;
};