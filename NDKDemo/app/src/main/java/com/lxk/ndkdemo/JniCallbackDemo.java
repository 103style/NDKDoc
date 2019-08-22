package com.lxk.ndkdemo;

import android.os.Build;
import android.util.Log;

import androidx.annotation.Keep;

/**
 * @author https://github.com/103style
 * @date 2019/8/22 10:42
 * 演示在C中如何调用Java
 */
public class JniCallbackDemo {

    private static final String TAG = "JniCallbackDemo";

    static {
        System.loadLibrary("jni_callback");
    }

    /**
     * 时间统计
     */
    private int timeCount;

    /**
     * 开始计时
     */
    public native void startTiming();

    /**
     * 停止计时
     */
    public native void stopTiming();

    /**
     * 输出开始计时后的每一秒
     */
    @Keep
    private void printTime() {
        Log.e(TAG, "timeCount = " + timeCount);
        timeCount++;
    }


    /**
     * Keep ：保证类名不被混淆
     */
    @Keep
    public static class JniHandler {

        /**
         * Keep ：保证方法名不被混淆
         */
        @Keep
        public static String getBuildVersion() {
            return Build.VERSION.RELEASE;
        }

        @Keep
        public long getRuntimeMemorySize() {
            return Runtime.getRuntime().freeMemory();
        }

        @Keep
        private void updateStatus(String msg) {
            if (msg.toLowerCase().contains("error")) {
                Log.e("JniHandler", "Native Err: " + msg);
            } else {
                Log.i("JniHandler", "Native Msg: " + msg);
            }
        }
    }
}

