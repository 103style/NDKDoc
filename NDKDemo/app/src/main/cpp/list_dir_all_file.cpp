//
// Created by xiaoke.luo on 2019/8/21.
//

#include <jni.h>
#include <dirent.h>
#include <string>
#include <android/log.h>
#include "LogUtils.h"

const int PATH_MAX_LENGTH = 256;

using namespace std;
extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_ndkdemo_JniListDirAllFiles_listDirAllFile(JNIEnv *env, jobject instance,
                                                       jstring dirPath_) {
    //空判断
    if (dirPath_ == nullptr) {
        LOGE("dirPath is null!");
        return;
    }
    const char *dirPath = env->GetStringUTFChars(dirPath_, nullptr);
    //长度判读
    if (strlen(dirPath) == 0) {
        LOGE("dirPath length is 0!");
        return;
    }
    //打开文件夹读取流
    DIR *dir = opendir(dirPath);
    if (nullptr == dir) {
        LOGE("can not open dir,  check path or permission!")
        return;
    }

    struct dirent *file;
    while ((file = readdir(dir)) != nullptr) {
        //判断是不是 . 或者 .. 文件夹
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
            LOGV("ignore . and ..");
            continue;
        }

        if (file->d_type == DT_DIR) {
            //是文件夹则遍历
            //构建文件夹路径
            char *path = new char[PATH_MAX_LENGTH];
            memset(path, 0, PATH_MAX_LENGTH);
            strcpy(path, dirPath);
            strcat(path, "/");
            strcat(path, file->d_name);
            jstring tDir = env->NewStringUTF(path);
            //读取指定文件夹
            Java_com_lxk_ndkdemo_JniListDirAllFiles_listDirAllFile(env, instance, tDir);
            //释放文件夹路径内存
            free(path);
        } else {
            //打印文件名
            LOGD("%s/%s", dirPath, file->d_name);
        }
    }

    //关闭读取流
    closedir(dir);

    env->ReleaseStringUTFChars(dirPath_, dirPath);
}



