//
// Created by xiaoke.luo on 2019/8/19.
//


#include <jni.h>
#include <random>


//数组元素最大值
const jint max = 100;

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_example_myapplication_JniArrayOperation_getIntArray(JNIEnv *env, jobject instance,
                                                             jint length) {
    //创建一个指定大小的数组
    jintArray array = env->NewIntArray(length);

    jint *elementsP = env->GetIntArrayElements(array, nullptr);

    //设置 0-100的随机元素
    jint *startP = elementsP;
    for (; startP < elementsP + length; startP++) {
        *startP = static_cast<jint>(random() % max);
    }
    env->ReleaseIntArrayElements(array, elementsP, 0);
    return array;
}


int compare(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JniArrayOperation_sortIntArray(JNIEnv *env, jobject instance,
                                                              jintArray arr_) {
    //获取数组起始元素的指针
    jint *arr = env->GetIntArrayElements(arr_, nullptr);
    //获取数组长度
    jint len = env->GetArrayLength(arr_);
    //排序
    qsort(arr, len, sizeof(jint), compare);

    //第三个参数 同步
    //0：Java数组进行更新，并且释放C/C++数组
    //JNI_ABORT：Java数组不进行更新，但是释放C/C++数组
    //JNI_COMMIT：Java数组进行更新，不释放C/C++数组(函数执行完后，数组还是会释放的)
    env->ReleaseIntArrayElements(arr_, arr, 0);
}