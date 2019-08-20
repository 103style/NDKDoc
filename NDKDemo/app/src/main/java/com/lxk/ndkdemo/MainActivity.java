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

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.bt_array_test).setOnClickListener(this);
        findViewById(R.id.bt_encryptor_test).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        int id = view.getId();
        if (id == R.id.bt_array_test) {
            testJniArray();
        } else if (id == R.id.bt_encryptor_test) {
            testEncryptor();
        }
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
