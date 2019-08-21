package com.lxk.ndkdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.bt_array_test).setOnClickListener(this);
        findViewById(R.id.bt_encryptor_test).setOnClickListener(this);
        findViewById(R.id.bt_file_operation_test).setOnClickListener(this);
        findViewById(R.id.bt_list_dir_all_file).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        int id = view.getId();
        if (id == R.id.bt_array_test) {
            testJniArray();
        } else if (id == R.id.bt_encryptor_test) {
            testEncryptor();
        } else if (id == R.id.bt_file_operation_test) {
            testFileOperation();
        } else if (id == R.id.bt_list_dir_all_file) {
            testListDirAllFiles();
        }
    }

    /**
     * jni数组操作
     */
    private void testJniArray() {
        new JniArrayOperation().test();
    }

    private boolean hasFilePermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0x1024);
                return false;
            }
        }
        return true;
    }

    /**
     * 测试jni文件加解密
     */
    private void testEncryptor() {
        if (hasFilePermission()) {
            String encryptPath = new JniFileEncryptor().test();
            Toast.makeText(this, "加密文件地址：" + encryptPath, Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 测试jni文件查分和合并
     */
    private void testFileOperation() {
        if (hasFilePermission()) {
            new JniFileOperation().test();
            Toast.makeText(this, "任务完成，测试文件路径:" + Config.getBaseUrl(), Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 测试jni文件夹遍历
     */
    private void testListDirAllFiles() {
        if (hasFilePermission()) {
            new JniListDirAllFiles().test();
            Toast.makeText(this, "任务完成，检查看日志输出", Toast.LENGTH_SHORT).show();
        }
    }

}
