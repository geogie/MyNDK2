package com.georgeren.myndk2;

import android.view.Surface;

/**
 * Created by georgeRen on 2017/9/22.
 * yuv:YUV视频像素数据转换为RGB数据
 * FFmpeg库一共包含8个：avcodec,avformat,avfilter,avdevice,avutil,postproc,swresample,swscale
 */

public class VideoPlayer {// yuv 没用，但是cmake里添加了。

    static {
        System.loadLibrary("avutil-54");// 工具库（大部分库都需要这个库的支持）
        System.loadLibrary("swresample-1");// 音频采样数据格式转换
        System.loadLibrary("avcodec-56");// 编解码（最重要的库）
        System.loadLibrary("avformat-56");// 封装格式处理
        System.loadLibrary("swscale-3");// 视频像素数据格式转换
        System.loadLibrary("postproc-53");// 后加工
        System.loadLibrary("avfilter-5");// 滤镜特效处理。
        System.loadLibrary("avdevice-56");// 各种设备的输入输出
        System.loadLibrary("native-lib");// 我自己的封装库
    }

    public native void render(String input, Surface surface);
}
