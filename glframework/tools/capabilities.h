#include <string>
#include <iostream>
#include <unordered_map>
#include <cassert>

#include "../core.h"
#include "../../wrapper/checkError.h"

class GLCapabilities
{
public:
    // Initialize the GPU capabilities (call once at application start)
    static void Initialize();

    // Get GPU limits
    static int GetMaxColorAttachments()
    {
        return maxColorAttachments;
    }

    static int GetMaxDrawBuffers()
    {
        return maxDrawBuffers;
    }

    static int GetMaxTextureSize()
    {
        return maxTextureSize;
    }

private:
    // Check if a specific OpenGL extension is supported
    static bool CheckExtension(const std::string& extensionName);
    // Log all queried capabilities
    static void LogCapabilities();

    // Capability and limit data
    static bool arbframebufferSupported;
    static bool packedDepthStencilSupported;
    static int maxColorAttachments;
    static int maxDrawBuffers;
    static int maxTextureSize;
};