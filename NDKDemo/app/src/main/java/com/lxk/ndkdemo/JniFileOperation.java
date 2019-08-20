package com.lxk.ndkdemo;

/**
 * @author https://github.com/103style
 * @date 2019/8/20 14:13
 * 通过JNI实现文件操作  文件拆分和合并
 */
public class JniFileOperation {

    static {
        System.loadLibrary("file_operation");
    }

    /**
     * 拆分
     *
     * @param path        文件路径
     * @param pathPattern 拆分之后文件的路径格式
     * @param splitCount  拆分成几个
     */
    public native static void split(String path, String pathPattern, int splitCount);

    /**
     * 合并
     *
     * @param pathMerge   合并之后的文件路径
     * @param pathPattern 要合并的文件的路径格式
     * @param count       要合并的文件数量
     */
    public native static void merge(String pathMerge, String pathPattern, int count);
}
