#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "openGLRenderer.h"

#define REND (RendererManager::getInstance())

class RendererManager
{
private:
    std::unique_ptr<OpenGLRenderer> activeRenderer;
    RendererManager() = default;

    // Delete copy constructor and assignment operator
    RendererManager(const RendererManager&) = delete;
    RendererManager& operator=(const RendererManager&) = delete;

public:
    ~RendererManager() = default;

    static RendererManager& getInstance();

    // Set the active renderer
    void setRenderer(std::unique_ptr<OpenGLRenderer> renderer);

    // Clear the active renderer
    void clearRenderer();

    // Set the clear color for the active renderer
    void setClearColor(glm::vec3 color);

    // Prepare the scene for rendering
    void prepareScene();

    // Render the scene
    void render();
};