package com.lxk.ndkdemo;

import android.util.Log;

import java.io.File;

/**
 * @author https://github.com/103style
 * @date 2019/8/20 14:13
 * 通过JNI实现文件操作  文件拆分和合并
 */
public class JniFileOperation {

    private static final String TAG = "JniFileOperation";

    static {
        System.loadLibrary("file_operation");
    }

    /**
     * 原文件名
     */
    private String fileName = "split_test.txt";
    /**
     * 合并拆分之后文件的文件名
     */
    private String mergeFileName = "split_test_merged.txt";

    /**
     * 文件拆分格式
     */
    private String splitFileFormat = "split_test_%d.txt";

    /**
     * 拆分的数量
     */
    private int splitCount = 4;

    public native void createFile(String fileName);

    /**
     * 拆分
     *
     * @param path        文件路径
     * @param pathPattern 拆分之后文件的路径格式
     * @param splitCount  拆分成几个
     */
    public native void split(String path, String pathPattern, int splitCount);

    /**
     * 合并
     *
     * @param pathMerge   合并之后的文件路径
     * @param pathPattern 要合并的文件的路径格式
     * @param count       要合并的文件数量
     */
    public native void merge(String pathMerge, String pathPattern, int count);

    /**
     * 测试文件 拆分与合并
     */
    public void test() {
        String filePath = Config.getBaseUrl() + fileName;

        Log.e(TAG, "filePath = " + filePath);

        File file = new File(filePath);
        if (!file.exists()) {
            Log.e(TAG, "开始创建文件");
            createFile(filePath);
        }

        String pathPattern = Config.getBaseUrl() + splitFileFormat;
        split(filePath, pathPattern, splitCount);
        Log.e(TAG, "文件拆分成功");


        String mergePath = Config.getBaseUrl() + mergeFileName;
        merge(mergePath, pathPattern, splitCount);
        Log.e(TAG, "文件合并成功");
    }
}
