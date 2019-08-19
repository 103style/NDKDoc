//
// Created by 103style on 2019/8/15.
//


#include <jni.h>
#include "com_example_myapplication_MainActivity.h"


JNIEXPORT jstring JNICALL Java_com_example_myapplication_MainActivity_helloNDK
        (JNIEnv *env, jobject) {
    return env->NewStringUTF("Hello NDK");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_MainActivity_accessField(JNIEnv *env, jobject instance) {
    //获取类
    jclass jcla = env->GetObjectClass(instance);
    //获取类的属性id
    jfieldID jfID = env->GetFieldID(jcla, "showText", "Ljava/lang/String;");

    jstring after = env->NewStringUTF("Hello NDK");
    //修改属性id对应的值
    env->SetObjectField(instance, jfID, after);
}