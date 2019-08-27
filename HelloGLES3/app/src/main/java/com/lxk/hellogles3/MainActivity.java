package com.lxk.hellogles3;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

/**
 * @author https://github.com/103style
 * @date 2019/8/26 11:11
 */
public class MainActivity extends AppCompatActivity {

    private GLES3JNIView view;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        view = new GLES3JNIView(getApplication());
        setContentView(view);
    }


    @Override
    protected void onResume() {
        super.onResume();
        view.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        view.onPause();
    }
}
