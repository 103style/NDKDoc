package com.example.myapplication;

/**
 * @author https://github.com/103style
 * @date 2019/8/19 16:01
 * jni 数据操作
 */
public class JniArrayOperation {

    static {
        System.loadLibrary("array_operation");
    }

    public native int[] getIntArray(int length);

    public native void sortIntArray(int[] arr);

    public void test() {
        //获取随机的20个数
        int[] array = getIntArray(20);
        for (int i : array) {
            System.out.print(i + ", ");
        }
        System.out.println();

        //对数组进行排序
        sortIntArray(array);

        System.out.println("After sort:");
        for (int i : array) {
            System.out.print(i + ", ");
        }
        System.out.println();
    }
}


