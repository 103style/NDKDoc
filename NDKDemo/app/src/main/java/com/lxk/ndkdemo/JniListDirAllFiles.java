package com.lxk.ndkdemo;

/**
 * @author https://github.com/103style
 * @date 2019/8/21 10:14
 * <p>
 * 打印一个目录下的所有文件夹
 */
public class JniListDirAllFiles {

    static {
        System.loadLibrary("list_dir_all_file");
    }

    /**
     * 输出文件夹下得所有文件
     *
     * @param dirPath 文件夹路径
     */
    public native void listDirAllFile(String dirPath);


    public void test() {
        listDirAllFile(Config.BASE_URL);
    }
}
