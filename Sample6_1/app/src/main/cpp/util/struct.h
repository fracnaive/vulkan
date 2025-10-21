//
// Created by zheng on 2025/10/21.
//

#ifndef SAMPLE6_1_STRUCT_H
#define SAMPLE6_1_STRUCT_H

#include <vulkan/vulkan.h>

struct VulkanDeviceConfig {
    VkBool32 enableDepthClamp;
    VkBool32 enableAnisotropy;
};

#endif //SAMPLE6_1_STRUCT_H
