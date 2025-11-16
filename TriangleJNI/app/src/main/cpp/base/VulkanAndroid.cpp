/*
* Android Vulkan function pointer loader
*
* Copyright (C) 2016-2025 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "VulkanAndroid.h"

#if defined(__ANDROID__)
	#include <android/log.h>
	#include <dlfcn.h>
	#include <android/native_window_jni.h>

ANativeWindow* window;
uint32_t screenWidth;
uint32_t screenHeight;
AAssetManager* assetManager;
int32_t vks::android::screenDensity;

void *libVulkan;

namespace vks
{
	namespace android
	{
		// Dynamically load Vulkan library and base function pointers
		bool loadVulkanLibrary()
		{
			__android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "Loading libvulkan.so...\n");

			// Load vulkan library
			libVulkan = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
			if (!libVulkan)
			{
				__android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "Could not load vulkan library : %s!\n", dlerror());
				return false;
			}
			return true;
		}

		// Load instance based Vulkan function pointers
		void loadVulkanFunctions(VkInstance instance)
		{
			__android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "Loading instance based function pointers...\n");

        }

		void freeVulkanLibrary()
		{
			dlclose(libVulkan);
		}

		void getDeviceConfig(int32_t density)
		{
			vks::android::screenDensity = density;
		}

		// Displays a native alert dialog using JNI
		void showAlert(const char* message) {
//			JNIEnv* jni;
//			androidApp->activity->vm->AttachCurrentThread(&jni, NULL);
//
//			jstring jmessage = jni->NewStringUTF(message);
//
//			jclass clazz = jni->GetObjectClass(androidApp->activity->clazz);
//			// Signature has to match java implementation (arguments)
//			jmethodID methodID = jni->GetMethodID(clazz, "showAlert", "(Ljava/lang/String;)V");
//			jni->CallVoidMethod(androidApp->activity->clazz, methodID, jmessage);
//			jni->DeleteLocalRef(jmessage);
//
//			androidApp->activity->vm->DetachCurrentThread();
		}

        void setWindow(ANativeWindow* nativeWindow) {
            window = nativeWindow;
            if (window) {
                screenWidth = ANativeWindow_getWidth(window);
                screenHeight = ANativeWindow_getHeight(window);
                LOGE("窗口宽度%d， 窗口高度%d", screenWidth, screenHeight);
            } else {
                LOGE("设置窗口失败： 窗口指针为空");
            }
        }

        void setAssetManager(AAssetManager* manager) {
            assetManager = manager;
        }
	}
}

#endif
