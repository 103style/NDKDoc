package com.lxk.ndkdemo;

import android.os.Environment;
import android.text.TextUtils;

import java.io.File;

/**
 * @author https://github.com/103style
 * @date 2019/8/20 16:09
 * 配置类
 */
public class Config {

    public static final String BASE_URL = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;

    private static String DIR_NAME = "NDKDemo";

    private static String DIR_PATH;

    /**
     * 获取文件存储目录
     */
    public static String getBaseUrl() {
        if (!TextUtils.isEmpty(DIR_PATH)) {
            return DIR_PATH;
        }
        File file = new File(BASE_URL + DIR_NAME);
        if (!file.exists()) {
            file.mkdir();
        }
        DIR_PATH = file.getAbsolutePath() + File.separator;
        return DIR_PATH;
    }
}
