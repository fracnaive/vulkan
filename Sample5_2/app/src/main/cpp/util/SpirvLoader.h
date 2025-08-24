//
// Created by zheng on 2025/8/1.
//

#ifndef DEMO2_SPIRVLOADER_H
#define DEMO2_SPIRVLOADER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <android/asset_manager.h>

class SpirvLoader {
private:
    static AAssetManager* aam;
public:
    static void setAAssetManager(AAssetManager* aamIn);

    /**
     * 从assets目录加载SPIR-V文件
     * @param assetManager AAssetManager指针
     * @param filePath assets中的SPV文件路径（如"shaders/vertex.spv"）
     * @return 存储SPIR-V二进制数据的向量
     */
    static std::vector<uint32_t> loadFromAssets(const std::string& filePath);

    /**
     * 从SPIR-V二进制数据创建Vulkan着色器模块
     * @param device Vulkan逻辑设备
     * @param spirvData SPIR-V二进制数据
     * @param shaderModule 输出的着色器模块
     * @return 成功返回true，失败返回false
     */
    static bool createShaderModule(VkDevice device, const std::vector<uint32_t>& spirvData, VkShaderModule& shaderModule);
};


#endif //DEMO2_SPIRVLOADER_H
