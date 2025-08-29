//
// Created by zheng on 2025/8/30.
//
#include "TexDrawableObject.h"
#include "mylog.h"
#include <cassert>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <cstring>

TexDrawableObject::TexDrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
: devicePointer(&device), vdata(vdataIn), vCount(vCountIn)
{
    pushConstantData=new float[16];
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = nullptr;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = dataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = nullptr;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vkCreateBuffer(device, &buf_info, nullptr, &vertexDatabuf);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(device, vertexDatabuf, &mem_reqs);
    assert(dataByteCount<=mem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = nullptr;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    assert(flag);
    LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
    result = vkAllocateMemory(device, &alloc_info, nullptr, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, dataByteCount);
    vkUnmapMemory(device, vertexDataMem);
    result = vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}
TexDrawableObject::~TexDrawableObject()
{
    delete vdata;
    vkDestroyBuffer(*devicePointer, vertexDatabuf, nullptr);
    vkFreeMemory(*devicePointer, vertexDataMem, nullptr);
}
void TexDrawableObject::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout, VkPipeline& pipeline,VkDescriptorSet* desSetPointer){ //绘制物体的方法
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline); //将当前使用的命令缓冲与管线绑定
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,desSetPointer, 0, nullptr); //将命令缓冲、管线布局、描述集绑定
    const VkDeviceSize offsetsVertex[1] = {0}; //顶点数据偏移量数组
    vkCmdBindVertexBuffers( //将顶点数据与当前使用的命令缓冲绑定
            cmd, //当前使用的命令缓冲
            0, //顶点数据缓冲在列表中的首索引
            1, //绑定顶点缓冲的数量
            &(vertexDatabuf), //绑定的顶点数据缓冲列表
            offsetsVertex //各个顶点数据缓冲的内部偏移量
    );
    float* mvp=MatrixState3D::getFinalMatrix(); //获取最终变换矩阵
    memcpy(pushConstantData, mvp, sizeof(float)*16); //将最终变换矩阵数据送入推送常量数据
    vkCmdPushConstants(cmd, pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float)*16,pushConstantData); //将最终变换矩阵数据送入推送常量
    vkCmdDraw(cmd, vCount, 1, 0, 0); //执行绘制
}

