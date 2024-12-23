#include "RendererManager.h"
#include <assert.h>

RendererManager& RendererManager::getInstance()
{
    // Guaranteed to be thread-safe in C++11 and later
    static RendererManager mInstance;
    return mInstance;
}

// Add a renderer to the manager
void RendererManager::addRenderer(std::unique_ptr<OpenGLRenderer> renderer)
{
    renderers.push_back(std::move(renderer));
}

void RendererManager::removeRenderer(size_t index)
{
    // When an element is erased, the std::unique_ptr destructor is
    // automatically called, releasing the managed resource.
    if (index < renderers.size())
    {
        renderers.erase(renderers.begin() + index);
    }
}

void RendererManager::removeAll()
{
    renderers.clear();
}

// Render all scenes

void RendererManager::prepareScene()
{
    for (auto& renderer : renderers)
    {
        renderer->prepareScene();
    }
}

void RendererManager::renderAll()
{
    for (auto& renderer : renderers)
    {
        renderer->render();
    }
}

// Render a specific renderer by index
void RendererManager::renderByIndex(size_t index)
{
    if (index < renderers.size())
    {
        renderers[index]->render();
    }

    else
    {
        std::cerr << "Invalid renderer index.\n";
    }
}