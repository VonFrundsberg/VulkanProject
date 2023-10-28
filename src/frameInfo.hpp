#pragma once
#include "appCamera.hpp"
#include "objects/appObject.hpp"

#include <vulkan/vulkan.h>

namespace appNamespace{
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        AppCamera& camera;
        AppObject::Map& appObjects;
    };
} 