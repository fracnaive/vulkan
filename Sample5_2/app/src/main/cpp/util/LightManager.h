//
// Created by zheng on 2025/8/24.
//

#ifndef SAMPLE5_2_LIGHTMANAGER_H
#define SAMPLE5_2_LIGHTMANAGER_H

class LightManager {
public:

    static float lightAmbientR, lightAmbientG, lightAmbientB, lightAmbientA;
    static float lx, ly, lz; // 光源位置
    static float lightDiffuseR, lightDiffuseG, lightDiffuseB, lightDiffuseA; // 散色光强度
    // 设置环境光强度的方法
    static void setLightAmbient(float lightAmbientRIn, float lightAmbientGIn, float lightAmbientBIn, float lightAmbientAIn);
    static void setLightPosition(float lxIn, float lyIn, float lzIn); // 设置光源位置的方法
    static void setLightDiffuse(float lightDiffuseRIn, float lightDiffuseGIn, float lightDiffuseBIn, float lightDiffuseAIn);
};

#endif //SAMPLE5_2_LIGHTMANAGER_H
