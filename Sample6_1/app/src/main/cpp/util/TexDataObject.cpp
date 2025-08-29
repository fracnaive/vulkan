//
// Created by zheng on 2025/8/30.
//
#include "TexDataObject.h"

TexDataObject::TexDataObject(int width, int height, unsigned char *data, int dataByteCount)
: width(width), height(height), data(data), dataByteCount(dataByteCount) {

}

TexDataObject::~TexDataObject() {
    delete[] data;
}
