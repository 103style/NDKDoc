#include <jni.h>
#include <cstdio>
#include <cstdlib>

#include <android/log.h>


#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"com_lxk_ndkdemo",FORMAT,##__VA_ARGS__);

/**
 * 根据文件的路径，获得文件的大小
 */
long get_file_size(const char *path) {
    //rb：只读打开一个二进制文件，允许读数据
    //使用给定的模式 "rb" 打开 path 所指向的文件
    FILE *fp = fopen(path, "rb");
    if (fp == nullptr) {
        LOGE("%s", "文件打开失败");
        return 0;
    }
    //SEEK_SET	文件的开头
    //SEEK_CUR	文件指针的当前位置
    //SEEK_END	文件的末尾
    //设置流 fp 的文件位置为 0， 0 意味着从给定的 SEEK_END 位置查找的字节数。
    fseek(fp, 0, SEEK_END);
    //返回给定流 fp 的当前文件位置。
    return ftell(fp);
}


const int SIZE = 100;

/**
 * 创建拆分原文件
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniFileOperation_createFile(JNIEnv *env, jobject instance, jstring fileName_) {
    const char *fileName = env->GetStringUTFChars(fileName_, nullptr);

    //创建写文件流
    FILE *fp = fopen(fileName, "wb");

    //写文件
    for (int i = 0; i < SIZE; i++) {
        fputs("0123456789\n", fp);
    }
    //关闭流
    fclose(fp);
    LOGE("%s", "创建文件成功");
    env->ReleaseStringUTFChars(fileName_, fileName);
}

/**
 * 拆分文件
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniFileOperation_split(JNIEnv *env, jobject instance, jstring path_,
                                            jstring pathPattern_, jint splitCount) {
    const char *path = env->GetStringUTFChars(path_, nullptr);
    const char *pathPattern = env->GetStringUTFChars(pathPattern_, nullptr);

    //malloc：分配所需的内存空间，并返回一个指向它的指针。
    char **patches = new char *[splitCount];

    //获取文件长度
    long fileSize = get_file_size(path);

    //获取单个文件长度
    long per_size = fileSize / splitCount;

    //设置每个子文件的路径
    for (int i = 0; i < splitCount; i++) {
        patches[i] = new char[256];
        sprintf(patches[i], pathPattern, i);
        LOGE("patches[%d] = %s", i, patches[i]);
    }

    //创建fp流读取path对应的文件
    FILE *fp = fopen(path, "rb");
    if (fp == nullptr) {
        LOGE("%s", "文件打开失败");
        return;
    }
    //读取分割文件的流
    FILE *index_fp = nullptr;
    int index = 0;
    for (int i = 0; i < fileSize; i++) {
        if (i % per_size == 0) {
            if (index_fp != nullptr) {
                fclose(index_fp);
            }
            index_fp = fopen(patches[index], "wb");
            index++;
            if (index_fp == nullptr) {
                LOGE("文件%s打开失败", patches[index]);
                return;
            }
        }
        fputc(fgetc(fp), index_fp);

        //读完之后释放流
        if (i + 1 == fileSize) {
            fclose(index_fp);
        }
    }
    fclose(fp);

    //释放内存
    for (int i = 0; i < splitCount; i++) {
        free(patches[i]);
    }
    free(patches);

    env->ReleaseStringUTFChars(path_, path);
    env->ReleaseStringUTFChars(pathPattern_, pathPattern);
}

/**
 * 合并拆分文件
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniFileOperation_merge(JNIEnv *env, jobject instance, jstring pathMerge_,
                                            jstring pathPattern_, jint count) {
    const char *pathMerge = env->GetStringUTFChars(pathMerge_, nullptr);
    const char *pathPattern = env->GetStringUTFChars(pathPattern_, nullptr);

    //创建合并文件的写流
    FILE *fp = fopen(pathMerge, "wb");

    for (int i = 0; i < count; i++) {
        char *index = new char[256];
        sprintf(index, pathPattern, i);
        //读取每个分割文件
        FILE *index_fp = fopen(index, "rb");
        if (index_fp == nullptr) {
            LOGE("文件%s读取失败", index)
            return;
        }
        //依次写入合并文件
        int ch;
        while ((ch = fgetc(index_fp)) != EOF) {
            fputc(ch, fp);
        }
        //关闭当前的分割文件流
        fclose(index_fp);
        //释放拆分文件名的内存
        free(index);
    }
    fclose(fp);

    env->ReleaseStringUTFChars(pathMerge_, pathMerge);
    env->ReleaseStringUTFChars(pathPattern_, pathPattern);
}

