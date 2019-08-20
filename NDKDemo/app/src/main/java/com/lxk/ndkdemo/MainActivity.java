package com.lxk.ndkdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.IdRes;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        config(R.id.bt_array_test, new Runnable() {
            @Override
            public void run() {
                testJniArray();
            }
        });

        config(R.id.bt_encryptor_test, new Runnable() {
            @Override
            public void run() {
                testEncryptor();
            }
        });
    }

    private void config(@IdRes int id, final Runnable runnable) {
        findViewById(id).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                runnable.run();
            }
        });
    }

    /**
     * 测试jni文件加解密
     */
    private void testEncryptor() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0x1024);
                return;
            }
        }
        String encryptPath = new JniFileEncryptor().test();
        Toast.makeText(this, "加密文件地址：" + encryptPath, Toast.LENGTH_SHORT).show();
    }

    /**
     * jni数组操作
     */
    private void testJniArray() {
        new JniArrayOperation().test();
    }
}
