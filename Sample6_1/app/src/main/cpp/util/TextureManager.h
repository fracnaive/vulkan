//
// Created by zheng on 2025/8/30.
//

#ifndef SAMPLE6_1_TEXTUREMANAGER_H
#define SAMPLE6_1_TEXTUREMANAGER_H
#include <vector>
#include <map>
#include <string>
#include <vulkan/vulkan.h>
#include "TexDataObject.h"
#include "../bndev/mylog.h"
#include "struct.h"
#define SAMPLER_COUNT 1 //采样器数量
class TextureManager{
public:
    static std::vector<std::string> texNames; //纹理文件名称列表
    static std::vector<VkSampler> samplerList; //采样器列表
    static std::map<std::string,VkImage> textureImageList; //纹理图像列表
    static std::map<std::string,VkDeviceMemory> textureMemoryList; //纹理图像内存列表
    static std::map<std::string,VkImageView> viewTextureList; //纹理图像视图列表
    static std::map<std::string,VkDescriptorImageInfo> texImageInfoList; //纹理图像描述信息列表
    static VkBuffer stagingBuffer;
    static VkDeviceMemory stagingMemory;
    static void initTextures(VkDevice& device,VkPhysicalDevice& gpu,
                             VkPhysicalDeviceMemoryProperties& memoryroperties,
                             VkCommandBuffer& cmdBuffer,VkQueue& queueGraphics, VulkanDeviceConfig& deviceConfig); //加载所有纹理的方法
    static void destroyTextures(VkDevice& device); //销毁所有纹理的方法
    static int getVkDescriptorSetIndex(std::string texName);//获取指定名称纹理在描述集列表中的索引
private:
    static void initSampler(VkDevice& device, VkPhysicalDevice& gpu); //初始化采样器的方法
    static void init_SPEC_2D_Textures(std::string texName, VkDevice& device, VkPhysicalDevice& gpu,
                                      VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer,
                                      VkQueue& queueGraphics,VkFormat format, TexDataObject* ctdo); //加载 2D 纹理的方法

    static void init_ASTC_2D_Textures(std::string texName, VkDevice& device, VkPhysicalDevice& gpu,
                                      VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer,
                                      VkFormat format, VulkanDeviceConfig& deviceConfig); //加载 ASTC 纹理的方法
    static void transitionImageLayout(
        VkCommandBuffer commandBuffer,
        VkImage image,
        VkFormat format,
        VkImageLayout oldLayout,
        VkImageLayout newLayout){
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        //根据oldLayout和newLayout设置访问掩码
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("unsupported layout transition!");
        }
        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage,
            destinationStage,
            0, 0, nullptr, 0, nullptr,
            1,
            &barrier);
    }
};

#endif //SAMPLE6_1_TEXTUREMANAGER_H
