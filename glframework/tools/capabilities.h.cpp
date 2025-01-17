#include "capabilities.h"


void GLCapabilities::Initialize()
{
    // Check for framebuffer object support
    arbframebufferSupported = GLCapabilities::CheckExtension("GL_ARB_framebuffer_object");
    packedDepthStencilSupported = GLCapabilities::CheckExtension("GL_EXT_packed_depth_stencil");

    // Query GPU limits
    GL_CALL(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments));
    GL_CALL(glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers));
    GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize));

    // Cache results in a log-friendly format
    GLCapabilities::LogCapabilities();
}

bool GLCapabilities::CheckExtension(const std::string& extensionName)
{
    const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
    if (extensions == nullptr)
    {
        std::cerr << "ERROR::GpuCapabilities:: Failed to retrieve OpenGL extensions." << std::endl;
        return false;
    }
    return strstr(extensions, extensionName.c_str()) != nullptr;
}

void GLCapabilities::LogCapabilities()
{
    std::cout << "GPU Capabilities:" << std::endl;
    std::cout << "  ARB Framebuffer Object Supported: " << (arbframebufferSupported ? "Yes" : "No") << std::endl;
    std::cout << "  Packed Depth Stencil Supported: " << (packedDepthStencilSupported ? "Yes" : "No") << std::endl;
    std::cout << "  Max Color Attachments: " << maxColorAttachments << std::endl;
    std::cout << "  Max Draw Buffers: " << maxDrawBuffers << std::endl;
    std::cout << "  Max Texture Size: " << maxTextureSize << std::endl;
}

// Define static members
bool GLCapabilities::arbframebufferSupported = false;
bool GLCapabilities::packedDepthStencilSupported = false;
int GLCapabilities::maxColorAttachments = 0;
int GLCapabilities::maxDrawBuffers = 0;
int GLCapabilities::maxTextureSize = 0;