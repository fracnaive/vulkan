//
// Created by zheng on 2025/8/1.
// glslc commonTexLight.vert -o shaders/commonTexLight.vert.spv -fshader-stage=vertex
// glslc commonTexLight.frag -o shaders/commonTexLight.frag.spv -fshader-stage=fragment
//
#include "SpirvLoader.h"
#include "mylog.h"
#include <cstring>

// 初始化静态成员
AAssetManager* SpirvLoader::aam = nullptr;

void SpirvLoader::setAAssetManager(AAssetManager* aamIn) {
    if (aamIn) {
        aam = aamIn;
    } else {
        // 可以根据需求选择抛出异常或记录日志
        throw std::invalid_argument("Invalid AAssetManager pointer");
    }
}

std::vector<uint32_t> SpirvLoader::loadFromAssets(const std::string& filePath) {
    std::vector<uint32_t> spirvData;

    // 检查输入参数
    if (!aam || filePath.empty()) {
        LOGE("无效的参数：AAssetManager为空或文件路径为空");
        return spirvData;
    }

    // 打开assets中的SPV文件
    AAsset* asset = AAssetManager_open(aam, filePath.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        LOGE("无法打开SPV文件：%s", filePath.c_str());
        return spirvData;
    }

    // 获取文件大小
    off_t fileSize = AAsset_getLength(asset);
    if (fileSize <= 0) {
        LOGE("SPV文件大小无效：%s，大小：%ld", filePath.c_str(), fileSize);
        AAsset_close(asset);
        return spirvData;
    }

    // 检查文件大小是否为4字节的倍数（SPIR-V指令是32位的）
    if (fileSize % sizeof(uint32_t) != 0) {
        LOGE("SPV文件大小不是4字节的倍数：%s，可能已损坏", filePath.c_str());
        AAsset_close(asset);
        return spirvData;
    }

    // 分配内存并读取文件内容
    size_t elementCount = fileSize / sizeof(uint32_t);
    spirvData.resize(elementCount);

    int bytesRead = AAsset_read(asset, spirvData.data(), fileSize);
    if (bytesRead != fileSize) {
        LOGE("SPV文件读取不完整：%s，预期：%ld，实际：%d", filePath.c_str(), fileSize, bytesRead);
        spirvData.clear();
    } else {
        LOGI("SPV文件加载成功：%s，指令数：%zu", filePath.c_str(), elementCount);
    }

    // 释放资源
    AAsset_close(asset);
    return spirvData;
}

bool SpirvLoader::createShaderModule(VkDevice device, const std::vector<uint32_t>& spirvData, VkShaderModule& shaderModule) {
    // 检查输入参数
    if (spirvData.empty() || !device) {
        LOGE("无效的参数：SPIR-V数据为空或设备指针无效");
        return false;
    }

    // 填充着色器模块创建信息
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirvData.size() * sizeof(uint32_t);
    createInfo.pCode = spirvData.data();

    // 创建着色器模块
    VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS) {
        LOGE("着色器模块创建失败，错误码：%d", result);
        return false;
    }

    LOGI("着色器模块创建成功，指令数：%zu", spirvData.size());
    return true;
}

int fromBytesToInt(unsigned char* buff){			//将字节序列转换为int值的方法
    int k=0;										//结果变量
    auto* temp=(unsigned char*)(&k);		//将结果变量所占内存以字节序列模式访问
    temp[0]=buff[0];								//设置第1个字节的数据
    temp[1]=buff[1];								//设置第2个字节的数据
    temp[2]=buff[2]; 								//设置第3个字节的数据
    temp[3]=buff[3]; 								//设置第4个字节的数据
    return k;										//返回结果值
}

TexDataObject* SpirvLoader::loadCommonTexData(const string& fname){			//加载bntex纹理数据
    AAsset* asset =AAssetManager_open(aam,fname.c_str(),
                                      AASSET_MODE_UNKNOWN);			//创建AAsset对象
    auto* buf = new unsigned char[4];						//开辟长度为4字节的内存
    AAsset_read(asset, (void*)buf, 4);								//读取纹理宽度数据字节
    int width=fromBytesToInt(buf);									//转换为int型数值
    AAsset_read(asset, (void*)buf, 4);								//读取纹理高度数据字节
    int height=fromBytesToInt(buf);									//转换为int型数值
    auto* data=new unsigned char[width*height*4];			//开辟纹理数据存储内存
    AAsset_read(asset, (void*)data, width*height*4);				//读取纹理数据
    auto* ctdo = new TexDataObject(width,height,data,width*height*4);//创建纹理数据对象
    return ctdo;													//返回结果
}

TexDataObject* SpirvLoader::loadCommonASTCTexData(const string& fname){			//加载bntex纹理数据
    AAsset* asset = AAssetManager_open(aam,fname.c_str(), AASSET_MODE_UNKNOWN);			//创建AAsset对象
    if (!asset) {
        throw std::runtime_error("[TextureManager] 无法打开assets下的astc文件：" + fname);
    }

    // 读取头部
    ASTCHeader header{};
    int headBytes = AAsset_read(asset, &header, sizeof(ASTCHeader));
    if (headBytes != sizeof(ASTCHeader)) {
        AAsset_close(asset);
        throw std::runtime_error("[TextureManager] 读取ASTC文件头失败！");
    }

    // 校验magic number
    if (header.magic[0] != 0x13 || header.magic[1] != 0xAB ||
        header.magic[2] != 0xA1 || header.magic[3] != 0x5C) {
        AAsset_close(asset);
        throw std::runtime_error("[TextureManager] 不是有效的astc文件！");
    }

    // 读取尺寸
    uint32_t width = header.xSize[0] | (header.xSize[1] << 8) | (header.xSize[2] << 16);
    uint32_t height = header.ySize[0] | (header.ySize[1] << 8) | (header.ySize[2] << 16);
    uint32_t depth = header.zSize[0] | (header.zSize[1] << 8) | (header.zSize[2] << 16);

    // 剩余全部都是压缩纹理数据
    off_t totalLen = AAsset_getLength(asset);
    size_t dataLen = totalLen - sizeof(ASTCHeader);
    auto* data = new unsigned char[dataLen];
    AAsset_read(asset, data, dataLen);

    AAsset_close(asset);

    // 后续交给显卡解码，无需自己解压
    auto* pTexDataObject = new TexDataObject((int)width, (int)height, data, (int)dataLen);
    return pTexDataObject;
}


