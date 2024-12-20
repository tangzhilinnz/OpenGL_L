#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "openGLRenderer.h"

#define REND (RendererManager::getInstance())

// RendererManager Singleton Class
class RendererManager
{
private:
    std::vector<std::unique_ptr<OpenGLRenderer>> renderers;

    // Private constructor to prevent instantiation
    RendererManager() = default;

    // Delete copy constructor and assignment operator
    RendererManager(const RendererManager&) = delete;
    RendererManager& operator=(const RendererManager&) = delete;

public:
    ~RendererManager() = default;

    static RendererManager& getInstance();

    // Add a renderer to the manager
    void addRenderer(std::unique_ptr<OpenGLRenderer> renderer);

    void prepareScene();
    void renderAll();
    void renderByIndex(size_t index);
};