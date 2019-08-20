//
// Created by xiaoke.luo on 2019/8/20.
//

#include <jni.h>

#include <android/log.h>
#include <cstdio>
#include <cstring>

#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"JniFileEncryptor",FORMAT,##__VA_ARGS__);

extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniFileEncryptor_createFile(JNIEnv *env, jobject instance,
                                                    jstring normalPath_) {
    //获取字符串保存在JVM中内存中
    const char *normalPath = env->GetStringUTFChars(normalPath_, nullptr);
    //打开 normalPath  wb:只写打开或新建一个二进制文件；只允许写数据
    FILE *fp = fopen(normalPath, "wb");

    //把字符串写入到指定的流 stream 中，但不包括空字符。
    fputs("Hi, this file is created by JNI, and my name is 103style.", fp);

    //关闭流 fp。刷新所有的缓冲区
    fclose(fp);
    //释放JVM保存的字符串的内存
    env->ReleaseStringUTFChars(normalPath_, normalPath);
}


char password[] = "103style";


extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniFileEncryptor_encryption(JNIEnv *env, jclass type, jstring normalPath_,
                                                    jstring encryptPath_) {
    //获取字符串保存在JVM中内存中
    const char *normalPath = env->GetStringUTFChars(normalPath_, nullptr);
    const char *encryptPath = env->GetStringUTFChars(encryptPath_, nullptr);

    LOGE("normalPath = %s, encryptPath = %s", normalPath, encryptPath);

    //rb:只读打开一个二进制文件，允许读数据。
    //wb:只写打开或新建一个二进制文件；只允许写数据
    FILE *normal_fp = fopen(normalPath, "rb");
    FILE *encrypt_fp = fopen(encryptPath, "wb");

    if (normal_fp == nullptr) {
        LOGE("%s", "文件打开失败");
        return;
    }

    //一次读取一个字符
    int ch = 0;
    int i = 0;
    size_t pwd_length = strlen(password);
    while ((ch = fgetc(normal_fp)) != EOF) { //End of File
        //写入(异或运算)
        fputc(ch ^ password[i % pwd_length], encrypt_fp);
        i++;
    }

    //关闭流 normal_fp和encrypt_fp。刷新所有的缓冲区
    fclose(normal_fp);
    fclose(encrypt_fp);

    //释放JVM保存的字符串的内存
    env->ReleaseStringUTFChars(normalPath_, normalPath);
    env->ReleaseStringUTFChars(encryptPath_, encryptPath);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniFileEncryptor_decryption(JNIEnv *env, jclass type, jstring encryptPath_,
                                                    jstring decryptPath_) {
    ////获取字符串保存在JVM中内存中
    const char *encryptPath = env->GetStringUTFChars(encryptPath_, nullptr);
    const char *decryptPath = env->GetStringUTFChars(decryptPath_, nullptr);

    LOGE("encryptPath = %s, decryptPath = %s", encryptPath, decryptPath);

    //rb:只读打开一个二进制文件，允许读数据。
    //wb:只写打开或新建一个二进制文件；只允许写数据
    FILE *encrypt_fp = fopen(encryptPath, "rb");
    FILE *decrypt_fp = fopen(decryptPath, "wb");

    if (encrypt_fp == nullptr) {
        LOGE("%s", "加密文件打开失败");
        return;
    }

    int ch;
    int i = 0;
    size_t pwd_length = strlen(password);
    while ((ch = fgetc(encrypt_fp)) != EOF) {
        fputc(ch ^ password[i % pwd_length], decrypt_fp);
        i++;
    }

    //关闭流 encrypt_fp 和 decrypt_fp。刷新所有的缓冲区
    fclose(encrypt_fp);
    fclose(decrypt_fp);

    //释放JVM保存的字符串的内存
    env->ReleaseStringUTFChars(encryptPath_, encryptPath);
    env->ReleaseStringUTFChars(decryptPath_, decryptPath);
}
