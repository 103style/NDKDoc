//
// Created by xiaoke.luo on 2019/8/21.
//

#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <cstring>
#include "LogUtils.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniBitmapDemo_passBitmap(JNIEnv *env, jobject instance, jobject bitmap) {

    if (nullptr == bitmap) {
        LOGE("bitmap is null");
    }

    AndroidBitmapInfo info;
    int result;

    //获取图片信息
    result = AndroidBitmap_getInfo(env, bitmap, &info);

    if (result != ANDROID_BITMAP_RESUT_SUCCESS) {
        LOGE("AndroidBitmap_getInfo failed, result: %d", result);
        return;
    }

    LOGD("bitmap width: %d, height: %d, format: %d, stride: %d", info.width, info.height,
         info.format, info.stride);

    unsigned char *addrPtr;

    // 获取像素信息
    result = AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&addrPtr));

    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_lockPixels failed, result: %d", result);
        return;
    }

    // 执行图片操作的逻辑
    int length = info.stride * info.height;
    for (int i = 0; i < length; ++i) {
        LOGD("value: %x", addrPtr[i]);
    }

    // 像素信息不再使用后需要解除锁定
    result = AndroidBitmap_unlockPixels(env, bitmap);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_unlockPixels failed, result: %d", result);
    }

}