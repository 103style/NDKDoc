package com.lxk.hellogles3;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @author https://github.com/103style
 * @date 2019/8/26 11:16
 */
public class GLES3Render implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("native-gles3");
    }

    public native void surfaceChanged(int w, int h);

    public native void drawFrame();

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) {
        surfaceChanged(w, h);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        drawFrame();
    }
}
