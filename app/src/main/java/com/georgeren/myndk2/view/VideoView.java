package com.georgeren.myndk2.view;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by georgeRen on 2017/9/25.
 * 设置下绘制色彩 RGBA_8888
 */

public class VideoView extends SurfaceView {
    public VideoView(Context context) {
        this(context, null, 0);
    }

    public VideoView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public VideoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        //初始化像素绘制的格式为RGBA_8888（色彩最丰富）
        SurfaceHolder holder = getHolder();
        holder.setFormat(PixelFormat.RGBA_8888);
    }
}
