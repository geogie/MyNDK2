package com.georgeren.myndk2;

/**
 * Created by georgeRen on 2017/9/25.
 */

public class VideoUtils {
    static {
        System.loadLibrary("avutil-54");//
        System.loadLibrary("swresample-1");
        System.loadLibrary("avcodec-56");//
        System.loadLibrary("avformat-56");//
        System.loadLibrary("swscale-3");//
        System.loadLibrary("postproc-53");//
        System.loadLibrary("avfilter-5");//
        System.loadLibrary("avdevice-56");//
        System.loadLibrary("native-lib");//
    }

    //视频解码
    public native static void decode(String input, String outptu);

    //音频解码
    public native static void sound(String input, String output);
}
