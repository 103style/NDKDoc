package com.lxk.ndkdemo;

import android.graphics.Bitmap;
import android.util.Log;

import java.nio.Buffer;
import java.nio.ByteBuffer;

/**
 * @author https://github.com/103style
 * @date 2019/8/21 14:30
 * <p>
 * jni使用bitmap
 */
    public class JniBitmapDemo {

        private static final String TAG = "JniBitmapDemo";

        static {
            System.loadLibrary("bitmap");
        }

        public native void passBitmap(Bitmap bitmap);


        public void test() {
            Bitmap bitmap = Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888);
            bitmap.eraseColor(0xff336699); // AARRGGBB
            byte[] bytes = new byte[bitmap.getWidth() * bitmap.getHeight() * 4];
            Buffer dst = ByteBuffer.wrap(bytes);
            bitmap.copyPixelsToBuffer(dst);
            // ARGB_8888 真实的存储顺序是 R-G-B-A
            Log.d(TAG, "R: " + Integer.toHexString(bytes[0] & 0xff));
            Log.d(TAG, "G: " + Integer.toHexString(bytes[1] & 0xff));
            Log.d(TAG, "B: " + Integer.toHexString(bytes[2] & 0xff));
            Log.d(TAG, "A: " + Integer.toHexString(bytes[3] & 0xff));

            passBitmap(bitmap);
        }
    }
