package com.example.myapplication;

import android.os.Environment;
import android.util.Log;

import java.io.File;

/**
 * @author https://github.com/103style
 * @date 2019/8/20 9:58
 * 加密工具类
 */
public class Encryptor {

    private static final String TAG = "Encryptor";

    static {
        System.loadLibrary("encryptor");
    }

    private final String BASE_URL = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;

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
        String normalPath = BASE_URL + fileName;
        File file = new File(normalPath);
        if (!file.exists()) {
            createFile(normalPath);
        }
        String encryptPath = BASE_URL + "encryption_" + fileName;
        Encryptor.encryption(normalPath, encryptPath);
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
        Encryptor.decryption(encryptPath, decryptPath);
        Log.d(TAG, "解密完成了...");
    }
}
