#pragma once
#include "appCamera.hpp"
#include "appObject.hpp"

#include <vulkan/vulkan.h>

namespace appNamespace{
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        AppCamera& camera;
        VkDescriptorSet globalDescriptorSet;
        AppObject::Map& appObjects;
    };
} 