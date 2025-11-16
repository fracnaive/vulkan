/*
* Android Vulkan function pointer prototypes
*
* Copyright (C) 2016-2025 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#ifndef VULKANANDROID_H
#define VULKANANDROID_H

// Vulkan needs to be loaded dynamically on android
// While SDK 26 (and up) come with a loader, we also want to support older devices, so we manually load function pointers

#pragma once

#ifndef VULKANANDROID_HPP
#define VULKANANDROID_HPP

#include "vulkan/vulkan.h"

#if defined(__ANDROID__)

#include <android/log.h>
#include <android/configuration.h>
#include <memory>
#include <string>

// Global reference to android application object
extern ANativeWindow* window;
extern uint32_t screenWidth;
extern uint32_t screenHeight;
extern AAssetManager* assetManager;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vulkanExample", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "vulkanExample", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "vulkanExample", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "vulkanExample", __VA_ARGS__))

namespace vks
{
	namespace android
	{
		/* @brief Touch control thresholds from Android NDK samples */
		const int32_t DOUBLE_TAP_TIMEOUT = 300 * 1000000;
		const int32_t TAP_TIMEOUT = 180 * 1000000;
		const int32_t DOUBLE_TAP_SLOP = 100;
		const int32_t TAP_SLOP = 8;

		/** @brief Density of the device screen (in DPI) */
		extern int32_t screenDensity;

		bool loadVulkanLibrary();
		void loadVulkanFunctions(VkInstance instance);
		void freeVulkanLibrary();
		void getDeviceConfig(int32_t density);
		void showAlert(const char* message);
        void setWindow(ANativeWindow* nativeWindow);
        void setAssetManager(AAssetManager* assetManager);
	}
}

#endif

#endif // VULKANANDROID_HPP


#endif // VULKANANDROID_H
 