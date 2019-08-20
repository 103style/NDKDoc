#include <jni.h>
#include <string>

extern "C"
JNIEXPORT void JNICALL Java_com_lxk_ndkdemo_JniFileOperation_split
        (JNIEnv *env, jclass type, jstring path, jstring pathPattern, jint splitCount) {

}


extern "C"
JNIEXPORT void JNICALL Java_com_lxk_ndkdemo_JniFileOperation_merge
        (JNIEnv *env, jclass type, jstring path, jstring pathPattern, jint count) {

}
