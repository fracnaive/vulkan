//
// Created by zheng on 2025/8/30.
//

#ifndef SAMPLE6_1_TEXDRAWABLEOBJECT_H
#define SAMPLE6_1_TEXDRAWABLEOBJECT_H
#include <vulkan/vulkan.h>
#include <string>
#include "TextureManager.h"
class TexDrawableObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf{};
    VkDeviceMemory vertexDataMem{};
    VkDescriptorBufferInfo vertexDataBufferInfo{};
    TexDrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TexDrawableObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};

#endif //SAMPLE6_1_TEXDRAWABLEOBJECT_H
