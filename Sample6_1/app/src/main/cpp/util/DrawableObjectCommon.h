#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H

#include <vulkan/vulkan.h>
#include <string>

class DrawableObjectCommonLight
{
public:
    float* pushConstantData; // 推送常量数据指针
    VkDevice* devicePointer;//指向逻辑设备的指针
    float* vdata;//顶点数据数组首地址指针
    int vCount;//顶点数量
    VkBuffer vertexDatabuf;//顶点数据缓冲
    VkDeviceMemory vertexDataMem;//顶点数据所需设备内存
    VkDescriptorBufferInfo vertexDataBufferInfo;//顶点数据缓冲描述信息
    // 构造函数：使用const引用传递参数，避免不必要的拷贝
    DrawableObjectCommonLight(
            float* vdataIn,
            int dataByteCount,
            int vCountIn,
            VkDevice& device,
            VkPhysicalDeviceMemoryProperties& memoryroperties
    );
    // 析构函数：声明为 noexcept 提高异常安全性
    ~DrawableObjectCommonLight() noexcept;
    // 绘制方法：参数使用const引用，明确不修改的对象
    void drawSelf(
            VkCommandBuffer& cmd,
            const VkPipelineLayout& pipelineLayout,
            const VkPipeline& pipeline,
            const VkDescriptorSet* desSetPointer
    );

    // 禁用拷贝构造和赋值操作（避免资源重复释放）
    DrawableObjectCommonLight(const DrawableObjectCommonLight&) = delete;
    DrawableObjectCommonLight& operator=(const DrawableObjectCommonLight&) = delete;

    // 允许移动构造和赋值（可选，根据需求决定）
    DrawableObjectCommonLight(DrawableObjectCommonLight&&) noexcept = default;
    DrawableObjectCommonLight& operator=(DrawableObjectCommonLight&&) noexcept = default;

private:
    void createVertexBuffer(int dataByteCount, VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryProperties);
};

#endif
