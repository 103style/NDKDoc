#include <jni.h>
#include <string>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <android/log.h>
#include "LogUtils.h"

static void printGLString(const char *name, GLenum s) {
    const char *glName = reinterpret_cast<const char *>(glGetString(s));
    LOGD("GL %s = %s", name, glName);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_hellogles3_GLES3Render_surfaceChanged(JNIEnv *env, jobject thiz, jint w, jint h) {
    printGLString("Version", GL_VERSION);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_hellogles3_GLES3Render_drawFrame(JNIEnv *env, jobject thiz) {

}