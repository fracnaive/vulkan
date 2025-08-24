//
// Created by zheng on 2025/8/24.
//

#ifndef SAMPLE5_2_LIGHTMANAGER_H
#define SAMPLE5_2_LIGHTMANAGER_H

class LightManager {
public:

    static float lightAmbientR, lightAmbientG, lightAmbientB, lightAmbientA;
    // 设置环境光强度的方法
    static void setLightAmbient(float lightAmbientRIn, float lightAmbientGIn, float lightAmbientBIn, float lightAmbientAIn);
};

#endif //SAMPLE5_2_LIGHTMANAGER_H
