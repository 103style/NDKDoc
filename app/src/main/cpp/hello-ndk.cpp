//
// Created by 103style on 2019/8/15.
//


#include <jni.h>
#include "com_example_myapplication_MainActivity.h"


JNIEXPORT jstring JNICALL Java_com_example_myapplication_MainActivity_helloNDK
  (JNIEnv * env, jobject){
    return env ->NewStringUTF("Hello NDK");
}