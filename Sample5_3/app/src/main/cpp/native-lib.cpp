#include <jni.h>
#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <string>
#include <vector>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <thread>
#include "bndev/MyVulkanManager.h"
#include "util/LightManager.h"
#include <android/input.h>

// 全局状态变量
static ANativeWindow* gWindow = nullptr;
static AAssetManager* gAssetManager = nullptr;
static std::atomic<bool> isRunning(false);
static std::thread renderThread;

// 触摸事件状态
static int xPre = 0;
static int yPre = 0;
static float xDis = 0.0f;
static float yDis = 0.0f;
static bool isClick = true;
static float TOUCH_SCALE_FACTOR = 180.0f / 320; // 角度缩放比例

extern "C" {
JNIEXPORT jboolean JNICALL
Java_com_nomk_sample5_13_MainActivity_handleInputEvent(JNIEnv *env, jobject thiz,
                                                       jint action, jint x, jint y, jint pointerCount) {
    switch (action)
    {
        case AMOTION_EVENT_ACTION_DOWN: // 触摸按下
            isClick = true; // 标记可能为点击
            xPre = x;       // 记录初始X坐标
            yPre = y;       // 记录初始Y坐标
            LOGI("zzz down: x=%d, y=%d", x, y);
            return JNI_TRUE;
        case AMOTION_EVENT_ACTION_MOVE: // 触摸移动
            // 计算位移
            xDis = x - xPre;
            yDis = y - yPre;
            // 判断是否超过滑动阈值
            if (abs((int)xDis) > 10 || abs((int)yDis) > 10)
            {
                isClick = false; // 超过阈值，视为滑动
            }
            // 如果是滑动操作，更新角度
            if (!isClick) {
                LightManager::lx = LightManager::lx + xDis;
                LightManager::ly = LightManager::ly - yDis;
                xPre = x; // 更新上一次X坐标
                yPre = y; // 更新上一次Y坐标
                LOGI("zzz move: 位移x=%.1f, y=%.1f, 角度=%.1f",
                     xDis, yDis, MyVulkanManager::yAngle);
            }
            return JNI_TRUE;
        case AMOTION_EVENT_ACTION_UP: // 触摸抬起
            if (isClick) // 如果是点击操作
            {
            }
            else // 滑动结束
            {
                LOGI("zzz move end: x=%d, y=%d", x, y);
            }
            return JNI_TRUE;
        default:
            return JNI_FALSE;
    }
}

JNIEXPORT jstring JNICALL
Java_com_nomk_sample5_13_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Vulkan 环境已就绪!";

    // 初始化 Vulkan
    VkInstance instance;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // 创建 Vulkan 实例
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        return env->NewStringUTF("Vulkan 初始化失败!");
    }

    // 获取可用的物理设备
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        vkDestroyInstance(instance, nullptr);
        return env->NewStringUTF("未找到支持 Vulkan 的 GPU!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    // 清理资源
    vkDestroyInstance(instance, nullptr);

    return env->NewStringUTF(hello.c_str());
}

// JNI方法：设置资源管理器（从Java的AssetManager转换）
JNIEXPORT void JNICALL
Java_com_nomk_sample5_13_MainActivity_setAssetManager(
        JNIEnv* env,
        jobject thiz,
        jobject assetManager) {
    gAssetManager = nullptr;
    if (assetManager) {
        gAssetManager = AAssetManager_fromJava(env, assetManager);
        if (gAssetManager) {
            LOGE("成功获取AAssetManager");
            MyVulkanManager::setAssetManager(gAssetManager); // 传递给Vulkan管理器
        } else {
            LOGE("获取AAssetManager失败");
        }
    }
}

JNIEXPORT void JNICALL
Java_com_nomk_sample5_13_MainActivity_initVulkan(JNIEnv *env, jobject thiz, jobject surface) {
    // 获取 NativeWindow
    gWindow = ANativeWindow_fromSurface(env, surface);
    MyVulkanManager::setWindow(gWindow);

    // 初始化Vulkan流程
    MyVulkanManager::doVulkan();
}
}