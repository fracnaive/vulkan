#include <jni.h>
#include <string>
#include "GLTFLoading.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <thread>
#include <vulkan/vulkan.h>

// 全局状态变量
static ANativeWindow* gWindow = nullptr;
static AAssetManager* gAssetManager = nullptr;
static std::unique_ptr<VulkanExample> vulkanExample = nullptr;
static std::unique_ptr<std::thread> renderThread;

extern "C" {
JNIEXPORT jboolean JNICALL
Java_com_nomk_trianglejni_MainActivity_handleInputEvent(JNIEnv *env, jobject thiz,
                                                       jint action, jint posX, jint posY, jint pointerCount, jlong eTime, jlong dTime) {
    if (vulkanExample == nullptr) {
        return 0;
    }
    switch (action) {
        case AMOTION_EVENT_ACTION_UP: {
            vulkanExample->lastTapTime = eTime;
            vulkanExample->touchPos.x = posX;
            vulkanExample->touchPos.y = posY;
            vulkanExample->touchTimer = 0.0;
            vulkanExample->touchDown = false;
            vulkanExample->camera.keys.up = false;

            // Detect single tap
            int64_t eventTime = eTime;
            int64_t downTime = dTime;
            if (eventTime - downTime <= vks::android::TAP_TIMEOUT) {
                float deadZone = (160.f / vks::android::screenDensity) * vks::android::TAP_SLOP * vks::android::TAP_SLOP;
                float x = posX - vulkanExample->touchPos.x;
                float y = posY - vulkanExample->touchPos.y;
                if ((x * x + y * y) < deadZone) {
                    vulkanExample->mouseState.buttons.left = true;
                }
            };

            return 1;
            break;
        }
        case AMOTION_EVENT_ACTION_DOWN: {
            // Detect double tap
            int64_t eventTime = eTime;
            if (eventTime - vulkanExample->lastTapTime <= vks::android::DOUBLE_TAP_TIMEOUT) {
                float deadZone = (160.f / vks::android::screenDensity) * vks::android::DOUBLE_TAP_SLOP * vks::android::DOUBLE_TAP_SLOP;
                float x = posX - vulkanExample->touchPos.x;
                float y = posY - vulkanExample->touchPos.y;
                if ((x * x + y * y) < deadZone) {
                    vulkanExample->keyPressed(TOUCH_DOUBLE_TAP);
                    vulkanExample->touchDown = false;
                }
            }
            else {
                vulkanExample->touchDown = true;
            }
            vulkanExample->touchPos.x = posX;
            vulkanExample->touchPos.y = posY;
            vulkanExample->mouseState.position.x = posX;
            vulkanExample->mouseState.position.y = posY;
            break;
        }
        case AMOTION_EVENT_ACTION_MOVE: {
            bool handled = false;
            if (vulkanExample->settings.overlay) {
                ImGuiIO& io = ImGui::GetIO();
                handled = io.WantCaptureMouse && vulkanExample->ui.visible;
            }
            if (!handled) {
                int32_t eventX = posX;
                int32_t eventY = posY;

                float deltaX = (float)(vulkanExample->touchPos.y - eventY) * vulkanExample->camera.rotationSpeed * 0.5f;
                float deltaY = (float)(vulkanExample->touchPos.x - eventX) * vulkanExample->camera.rotationSpeed * 0.5f;

                vulkanExample->camera.rotate(glm::vec3(deltaX, 0.0f, 0.0f));
                vulkanExample->camera.rotate(glm::vec3(0.0f, -deltaY, 0.0f));

                vulkanExample->touchPos.x = eventX;
                vulkanExample->touchPos.y = eventY;
            }
            break;
        }
        default:
            return 1;
            break;

    }
    return 1;
}

JNIEXPORT jstring JNICALL
Java_com_nomk_trianglejni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
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
Java_com_nomk_trianglejni_MainActivity_setAssetManager(
        JNIEnv* env,
        jobject thiz,
        jobject assetManager) {
    gAssetManager = nullptr;
    if (assetManager) {
        gAssetManager = AAssetManager_fromJava(env, assetManager);
        if (gAssetManager) {
            LOGE("成功获取AAssetManager");
            vks::android::setAssetManager(gAssetManager); // 传递给Vulkan管理器
        } else {
            LOGE("获取AAssetManager失败");
        }
    }
}

JNIEXPORT void JNICALL
Java_com_nomk_trianglejni_MainActivity_initVulkan(JNIEnv *env, jobject thiz, jobject surface) {
    // 获取 NativeWindow
    gWindow = ANativeWindow_fromSurface(env, surface);
    vks::android::setWindow(gWindow);
    vulkanExample = std::make_unique<VulkanExample>();
    if (vulkanExample->initVulkan()) {
        vulkanExample->prepare();
        renderThread = std::make_unique<std::thread>([=](){
           vulkanExample->renderLoop();
        });
    }
}

JNIEXPORT void JNICALL
Java_com_nomk_trianglejni_MainActivity_setScreenDensity(JNIEnv *env, jobject thiz, jfloat density) {
    vks::android::getDeviceConfig(density);
}

JNIEXPORT void JNICALL
Java_com_nomk_trianglejni_MainActivity_stopRenderLoop(JNIEnv *env, jobject thiz) {
    if (vulkanExample != nullptr) {
        vulkanExample->stopRenderLoop();
        if (renderThread && renderThread->joinable()) {
            LOGI("等待渲染线程退出");
            renderThread->join();
            LOGI("渲染线程已退出");
        }
        // 等待GPU命令执行结束
        vulkanExample->waitDevice();
        // (主动调用析构)
        vulkanExample.reset();
    }
}
}
