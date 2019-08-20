package com.lxk.ndkdemo;

import android.os.Environment;
import android.util.Log;

import java.io.File;

/**
 * @author https://github.com/103style
 * @date 2019/8/20 9:58
 * Jni实现文件加解密
 */
public class JniFileEncryptor {

    private static final String TAG = "JniFileEncryptor";

    static {
        System.loadLibrary("encryptor");
    }

    /**
     * 加密
     *
     * @param normalPath  文件路径
     * @param encryptPath 加密之后的文件路径
     */
    public native static void encryption(String normalPath, String encryptPath);

    /**
     * 解密
     *
     * @param encryptPath 加密之后的文件路径
     * @param decryptPath 解密之后的文件路径
     */
    public native static void decryption(String encryptPath, String decryptPath);

    /**
     * 创建文件
     *
     * @param normalPath 文件路径
     */
    private native void createFile(String normalPath);

    /**
     * 测试加解密
     */
    public String test() {
        String fileName = "testJni.txt";
        String encryptPath = encryption(fileName);
        decryption(encryptPath);
        return encryptPath;
    }

    /**
     * 加密
     */
    public String encryption(String fileName) {
        String normalPath = Config.getBaseUrl() + fileName;
        File file = new File(normalPath);
        if (!file.exists()) {
            createFile(normalPath);
        }
        String encryptPath = Config.getBaseUrl() + "encryption_" + fileName;
        JniFileEncryptor.encryption(normalPath, encryptPath);
        Log.d(TAG, "加密完成了...");
        return encryptPath;
    }

    /**
     * 解密
     */
    public void decryption(String encryptPath) {
        if (!new File(encryptPath).exists()) {
            Log.d(TAG, "解密文件不存在");
            return;
        }
        String decryptPath = encryptPath.replace("encryption_", "decryption_");
        JniFileEncryptor.decryption(encryptPath, decryptPath);
        Log.d(TAG, "解密完成了...");
    }
}
