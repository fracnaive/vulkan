cmake_minimum_required(VERSION 3.22.1)

project("triangle")

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

# 添加Vulkan头文件
find_package(Vulkan REQUIRED)

include_directories(${ANDROID_NDK}/sources/android/native_app_glue)

include_directories(base)
include_directories(external)
include_directories(external/glm)
include_directories(external/gli)
include_directories(external/imgui)
include_directories(external/tinygltf)

# 源文件自动收集
file(GLOB BASE_SRCS base/*.cpp)
file(GLOB EXTERNAL_SRCS external/*.cpp)
file(GLOB GLM_SRCS external/glm/*.cpp)
file(GLOB GLI_SRCS external/gli/*.cpp)
file(GLOB IMGUI_SRCS external/imgui/*.cpp)
file(GLOB TINYGLTF_SRCS external/tinygltf/*.cpp)

set(KTX_DIR external/ktx)
set(KTX_SOURCES
    ${KTX_DIR}/lib/texture.c
    ${KTX_DIR}/lib/hashlist.c
    ${KTX_DIR}/lib/checkheader.c
    ${KTX_DIR}/lib/swap.c
    ${KTX_DIR}/lib/memstream.c
    ${KTX_DIR}/lib/filestream.c
    ${KTX_DIR}/lib/vkloader.c
)
set(KTX_INCLUDE
    ${KTX_DIR}/include
    ${KTX_DIR}/lib
    ${KTX_DIR}/other_include
)

include_directories(${KTX_INCLUDE})

add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
        ${BASE_SRCS}
        ${EXTERNAL_SRCS}
        ${GLM_SRCS}
        ${GLI_SRCS}
        ${IMGUI_SRCS}
        ${TINYGLTF_SRCS}
        ${KTX_SOURCES}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        Vulkan::Vulkan
)