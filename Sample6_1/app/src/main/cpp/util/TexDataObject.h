//
// Created by zheng on 2025/8/30.
//

#ifndef SAMPLE6_1_TEXDATAOBJECT_H
#define SAMPLE6_1_TEXDATAOBJECT_H

class TexDataObject {
public:
    int width; // 纹理宽度
    int height; // 纹理高度
    int dataByteCount; // 纹理的数据总字节数
    unsigned char* data; // 指向纹理数据存储内存首地址的指针
    TexDataObject(int width, int height, unsigned char* data, int dataByteCount); // 构造函数
    ~TexDataObject(); //析构函数
};

#endif //SAMPLE6_1_TEXDATAOBJECT_H
