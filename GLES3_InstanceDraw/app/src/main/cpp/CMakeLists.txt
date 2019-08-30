cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")

add_library(
        Instancing
        SHARED
        esShader.c
        esShapes.c
        esTransform.c
        esUtil.c
        esUtil_Android.c
        Instancing.c
)


include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
add_library(app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_link_libraries(
        Instancing
        GLESv3
        android
        EGL
        log
        app_glue
        m
)