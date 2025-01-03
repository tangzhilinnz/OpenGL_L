#include "RendererManager.h"
#include <assert.h>
#include "../wrapper/checkError.h"

RendererManager& RendererManager::getInstance()
{
    // Guaranteed to be thread-safe in C++11 and later
    static RendererManager mInstance;
    return mInstance;
}

void RendererManager::setClearColor(glm::vec3 color)
{
    GL_CALL(glClearColor(color.r, color.g, color.b, 1.0f));
}

void RendererManager::setRenderer(std::unique_ptr<OpenGLRenderer> renderer)
{
    activeRenderer = std::move(renderer);
}

void RendererManager::clearRenderer()
{
    activeRenderer.reset();
}

void RendererManager::prepareScene() {
    if (activeRenderer)
    {
        activeRenderer->prepareScene();
    }
    else
    {
        std::cerr << "No active renderer to render." << std::endl;
        assert(false);
    }
}

void RendererManager::render()
{
    if (activeRenderer)
    {
        activeRenderer->render();
    }
    else
    {
        std::cerr << "No active renderer to render." << std::endl;
        assert(false);
    }
}