//
// Created by xiaoke.luo on 2019/8/22.
//

#include <jni.h>
#include <android/log.h>
#include <cstring>
#include <pthread.h>
#include <cassert>
#include <cinttypes>
#include "LogUtils.h"

typedef struct jni_callback {
    JavaVM *javaVM;
    jclass jniHandlerClz;
    jobject jniHandlerObj;
    jclass jniCallbackDemoClz;
    jobject jniCallbackDemoObj;
    pthread_mutex_t lock;
    int done;
} JniCallback;
JniCallback jniCallback;

void queryRuntimeInfo(JNIEnv *env) {

    //获取getBuildVersion的方法id
    jmethodID staticMethodId = env->GetStaticMethodID(jniCallback.jniHandlerClz, "getBuildVersion",
                                                      "()Ljava/lang/String;");
    if (!staticMethodId) {
        LOGE("Failed to retrieve getBuildVersion() methodID");
        return;
    }
    //执行静态方法获取getBuildVersion的方法id
    jstring releaseVersion = static_cast<jstring>(env->CallStaticObjectMethod(
            jniCallback.jniHandlerClz, staticMethodId));
    //获取字符串的地址
    const char *version = env->GetStringUTFChars(releaseVersion, nullptr);
    if (!version) {
        LOGE("Unable to get version string");
        return;
    }
    LOGD("releaseVersion = %s", version);
    //释放字符串的内存
    env->ReleaseStringUTFChars(releaseVersion, version);
    //删除引用
    env->DeleteLocalRef(releaseVersion);

    //获取非静态方法 getRuntimeMemorySize 的id
    jmethodID methodId = env->GetMethodID(jniCallback.jniHandlerClz, "getRuntimeMemorySize", "()J");
    if (!methodId) {
        LOGE("Failed to retrieve getRuntimeMemorySize() methodID");
        return;
    }
    //调用 getRuntimeMemorySize
    jlong freeMemorySize = env->CallLongMethod(jniCallback.jniHandlerObj, methodId);

    //打印可用内存
    LOGD("Runtime free memory size: %"
                 PRId64, freeMemorySize);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");
    JNIEnv *env;
    //给jniCallback初始化地址
    memset(&jniCallback, 0, sizeof(jniCallback));

    jniCallback.javaVM = vm;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    //获取JniCallbackDemo.JniHandler 类
    jclass clz = env->FindClass("com/lxk/ndkdemo/JniCallbackDemo$JniHandler");
    //赋值到结构体
    jniCallback.jniHandlerClz = static_cast<jclass>(env->NewGlobalRef(clz));
    if (!clz) {
        LOGE("FindClass JniCallbackDemo$JniHandler error");
        return JNI_ERR;
    }
    //获取构造函数
    jmethodID initMethodId = env->GetMethodID(jniCallback.jniHandlerClz, "<init>", "()V");
    //构建类
    jobject instance = env->NewObject(jniCallback.jniHandlerClz, initMethodId);
    if (!instance) {
        LOGE("NewObject jniHandler error")
        return JNI_ERR;
    }
    //赋值到结构体
    jniCallback.jniHandlerObj = env->NewGlobalRef(instance);

    //调用 JniHandler 的相关方法
    queryRuntimeInfo(env);

    jniCallback.done = 0;
    jniCallback.jniCallbackDemoObj = nullptr;
    return JNI_VERSION_1_6;
}

/**
 * 调用 类 instance 的 void方法 methodId
 */
void sendJavaMsg(JNIEnv *env, jobject instance, jmethodID methodId, const char *msg) {
    LOGD("jni sendJavaMsg");
    //获取字符串
    jstring javaMsg = env->NewStringUTF(msg);
    //调用对应方法
    env->CallVoidMethod(instance, methodId, javaMsg);
    //删除本地引用
    env->DeleteLocalRef(javaMsg);
}

void *StartTiming(void *context) {
    //获取参数
    JniCallback *JniCallback = static_cast<jni_callback *>(context);
    JavaVM *javaVm = JniCallback->javaVM;
    JNIEnv *env;

    jint res = javaVm->GetEnv((void **) (&env), JNI_VERSION_1_6);
    LOGD("javaVm->GetEnv() res = %d", res);
    if (res != JNI_OK) {
        //链接到虚拟机
        res = javaVm->AttachCurrentThread(&env, nullptr);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return nullptr;
        }
    } else {
        LOGE("javaVm GetEnv JNI_OK");
    }

    //获取 JniHandler 的 updateStatus 函数
    jmethodID statusId = env->GetMethodID(JniCallback->jniHandlerClz, "updateStatus",
                                          "(Ljava/lang/String;)V");

    sendJavaMsg(env, JniCallback->jniHandlerObj, statusId, "TimeThread status: initializing...");

    //获取 JniCallbackDemo 的 printTime 函数
    jmethodID timerId = env->GetMethodID(JniCallback->jniCallbackDemoClz, "printTime", "()V");

    //声明时间变量
    struct timeval beginTime, curTime, usedTime, leftTime;
    const struct timeval kOneSecond = {
            (__kernel_time_t) 1,
            (__kernel_suseconds_t) 0
    };

    sendJavaMsg(env, JniCallback->jniHandlerObj, statusId,
                "TimeThread status: prepare startTiming ...");

    while (true) {
        //获取当前的时间 赋值给 beginTime
        gettimeofday(&beginTime, nullptr);
        //占有互斥锁
        pthread_mutex_lock(&JniCallback->lock);
        //获取当前的状态
        int done = JniCallback->done;
        if (JniCallback->done) {
            JniCallback->done = 0;
        }
        //释放互斥锁
        pthread_mutex_unlock(&JniCallback->lock);

        if (done) {
            LOGD("JniCallback done");
            break;
        }

        //调用 printTime 函数
        env->CallVoidMethod(JniCallback->jniCallbackDemoObj, timerId);

        //获取当前的时间 赋值给 curTime
        gettimeofday(&curTime, nullptr);

        //计算函数运行消耗的时间
        //usedTime = curTime - beginTime
        timersub(&curTime, &beginTime, &usedTime);

        //计算需要等待的时间
        //leftTime = kOneSecond - usedTime
        timersub(&kOneSecond, &usedTime, &leftTime);

        //构建等待的时间
        struct timespec sleepTime;
        sleepTime.tv_sec = leftTime.tv_sec;
        sleepTime.tv_nsec = leftTime.tv_usec * 1000;

        if (sleepTime.tv_sec <= 1) {
            //睡眠对应纳秒的时间
            nanosleep(&sleepTime, nullptr);
        } else {
            sendJavaMsg(env, JniCallback->jniHandlerObj, statusId,
                        "TimeThread error: processing too long!");
        }
    }
    sendJavaMsg(env, JniCallback->jniHandlerObj, statusId,
                "TimeThread status: ticking stopped");
    //释放线程
    javaVm->DetachCurrentThread();
    return context;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniCallbackDemo_startTiming(JNIEnv *env, jobject instance) {

    LOGD("jni startTiming");

    //线程ID
    pthread_t threadInfo;
    //线程属性
    pthread_attr_t threadAttr;
    //初始化线程属性
    pthread_attr_init(&threadAttr);
    //设置脱离状态的属性
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    //互斥锁
    pthread_mutex_t lock;
    //初始化互斥锁
    pthread_mutex_init(&lock, nullptr);

    //获取当前类
    jclass clz = env->GetObjectClass(instance);
    //保存类和 实例 到 结构体中
    jniCallback.jniCallbackDemoClz = static_cast<jclass>(env->NewGlobalRef(clz));
    jniCallback.jniCallbackDemoObj = env->NewGlobalRef(instance);

    // StartTiming ：在线程中运行的函数  jniCallback 运行函数的参数
    int result = pthread_create(&threadInfo, &threadAttr, StartTiming, &jniCallback);
    assert(result == 0);
    //删除线程属性
    pthread_attr_destroy(&threadAttr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniCallbackDemo_stopTiming(JNIEnv *env, jobject instance) {
    LOGD("jni stopTiming");

    //占用互斥锁
    pthread_mutex_lock(&jniCallback.lock);
    //修改线程运行的条件
    jniCallback.done = 1;
    //释放互斥锁
    pthread_mutex_unlock(&jniCallback.lock);

    //初始化等待时间
    struct timespec sleepTime;
    memset(&sleepTime, 0, sizeof(sleepTime));
    sleepTime.tv_nsec = 100000000;

    while (jniCallback.done) {
        nanosleep(&sleepTime, nullptr);
    }

    //删除引用
    env->DeleteGlobalRef(jniCallback.jniCallbackDemoClz);
    env->DeleteGlobalRef(jniCallback.jniCallbackDemoObj);
    jniCallback.jniCallbackDemoObj = nullptr;
    jniCallback.jniCallbackDemoClz = nullptr;

    //删除互斥锁
    pthread_mutex_destroy(&jniCallback.lock);
}