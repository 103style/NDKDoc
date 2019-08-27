package com.lxk.hellogles3;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/**
 * @author https://github.com/103style
 * @date 2019/8/26 11:13
 */
public class GLES3JNIView extends GLSurfaceView {

    public GLES3JNIView(Context context) {
        this(context, null);
    }

    public GLES3JNIView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        setRenderer(new GLES3Render());
    }
}
