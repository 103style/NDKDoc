package com.example.myapplication;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import java.util.Date;
import java.util.Random;

/**
 * @author https://github.com/103style
 * @date 2019/8/19 14:55
 */
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    private static String staticString = "static string";

    static {
        System.loadLibrary("hello-ndk");
    }

    private String showText = "Hello World";

    private static int getRandomValue(int max) {
        return new Random().nextInt(max);
    }

    public native String helloNDK();

    public native void accessField();

    public native void accessStaticField();

    public native String accessMethod();

    public native Date accessConstructor();

    public native int accessStaticMethod(int max);

    public String getAuthName(String name) {
        Log.e(TAG, "name = " + name);
        return name;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        config();

        //调用jni获取返回值
        TextView show = findViewById(R.id.tv_show);
        show.setText(helloNDK());

        testJniAccess();

        testJniArray();

    }

    /**
     * jni数组操作
     */
    private void testJniArray() {
        new JniArrayOperation().test();
    }

    /**
     * jni访问java的变量和方法
     */
    private void testJniAccess() {
        TextView accessFiledShow = findViewById(R.id.tv_access_filed_show);
        String res = "before: " + showText;
        //通过ndk 修改成员变量
        accessField();
        res += ", after:" + showText;
        accessFiledShow.setText(res);

        TextView accessStaticFiledShow = findViewById(R.id.tv_static_access_filed_show);
        res = "before: " + staticString;
        //通过ndk 修改静态变量
        accessStaticField();
        res += ", after:" + staticString;
        accessStaticFiledShow.setText(res);

        //通过jni调用java方法 并返回结果
        TextView tvName = findViewById(R.id.tv_auth_name);
        res = "authName = " + accessMethod();
        tvName.setText(res);

        //通过jni调用java静态方法 并返回结果
        TextView staticMethodShow = findViewById(R.id.tv_static_method_show);
        res = "accessStaticMethod(100) = " + accessStaticMethod(100);
        staticMethodShow.setText(res);

        //通过jni调用构造方法实例化对象
        TextView staticConstShow = findViewById(R.id.tv_const_show);
        res = accessConstructor().toString();
        staticConstShow.setText(res);
    }

    private void config() {
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
