package com.georgeren.myndk2;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;
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
    public native void play(String input);

    /**
     * 创建一个AudioTrac对象，用于播放
     * Java_com_georgeren_myndk2_VideoPlayer_play中调用了这个方法。
     *
     * @return
     */
    public AudioTrack createAudioTrack(int sampleRateInHz, int nb_channels) {
        //固定格式的音频码流
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        Log.i("jason", "nb_channels:" + nb_channels);
        //声道布局
        int channelConfig;
        if (nb_channels == 1) {
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_MONO;
        } else if (nb_channels == 2) {
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_STEREO;
        } else {
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_STEREO;
        }

        int bufferSizeInBytes = AudioTrack.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat);

        AudioTrack audioTrack = new AudioTrack(
                AudioManager.STREAM_MUSIC,
                sampleRateInHz, channelConfig,
                audioFormat,
                bufferSizeInBytes, AudioTrack.MODE_STREAM);
        //播放
        //audioTrack.play();
        //写入PCM
        //audioTrack.write(audioData, offsetInBytes, sizeInBytes);
        //播放完调用stop即可

        audioTrack.stop();
        return audioTrack;
    }
}
