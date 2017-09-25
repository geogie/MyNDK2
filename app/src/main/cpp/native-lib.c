#include <jni.h>
#include <string.h>
#include <android/log.h>

#include "libavutil/time.h"
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"george",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"george",FORMAT,##__VA_ARGS__);



//使用这两个Window相关的头文件需要在CMake脚本中引入android库
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include "libyuv.h"

//视频解码
JNIEXPORT void JNICALL
Java_com_georgeren_myndk2_VideoPlayer_render(JNIEnv *env, jclass type, jstring input_,
                                            jstring surface) {

    //需要转码的视频文件(输入的视频文件)
    const char *input = (*env)->GetStringUTFChars(env, input_, 0);

    //1.注册所有组件，例如初始化一些全局的变量、初始化网络等等
    av_register_all();
    //avcodec_register_all();

    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    //2.打开输入视频文件
    if (avformat_open_input(&pFormatCtx, input, NULL, NULL) != 0) {
        LOGE("%s", "无法打开输入视频文件");
        return;
    }

    //3.获取视频文件信息，例如得到视频的宽高
    //第二个参数是一个字典，表示你需要获取什么信息，比如视频的元数据
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGE("%s", "无法获取视频文件信息");
        return;
    }

    //获取视频流的索引位置
    //遍历所有类型的流（音频流、视频流、字幕流），找到视频流
    int v_stream_idx = -1;
    int i = 0;
    //number of streams
    for (; i < pFormatCtx->nb_streams; i++) {
        //流的类型
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            v_stream_idx = i;
            break;
        }
    }

    if (v_stream_idx == -1) {
        LOGE("%s", "找不到视频流\n");
        return;
    }

    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    AVCodecContext *pCodecCtx = pFormatCtx->streams[v_stream_idx]->codec;
    //4.根据编解码上下文中的编码id查找对应的解码器
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    //（迅雷看看，找不到解码器，临时下载一个解码器，比如视频加密了）
    if (pCodec == NULL) {
        LOGE("%s", "找不到解码器，或者视频已加密\n");
        return;
    }

    //5.打开解码器，解码器有问题（比如说我们编译FFmpeg的时候没有编译对应类型的解码器）
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGE("%s", "解码器无法打开\n");
        return;
    }

    //准备读取
    //AVPacket用于存储一帧一帧的压缩数据（H264）
    //缓冲区，开辟空间
    AVPacket *packet = (AVPacket *) av_malloc(sizeof(AVPacket));

    //AVFrame用于存储解码后的像素数据(YUV)
    //内存分配
    AVFrame *yuv_frame = av_frame_alloc();
    AVFrame *rgb_frame = av_frame_alloc();

    int got_picture, ret;
    int frame_count = 0;

    //窗体
    ANativeWindow *pWindow = ANativeWindow_fromSurface(env, surface);
    //绘制时的缓冲区
    ANativeWindow_Buffer out_buffer;

    //6.一帧一帧的读取压缩数据
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        //只要视频压缩数据（根据流的索引位置判断）
        if (packet->stream_index == v_stream_idx) {
            //7.解码一帧视频压缩数据，得到视频像素数据
            ret = avcodec_decode_video2(pCodecCtx, yuv_frame, &got_picture, packet);
            if (ret < 0) {
                LOGE("%s", "解码错误");
                return;
            }

            //为0说明解码完成，非0正在解码
            if (got_picture) {

                //1、lock window
                //设置缓冲区的属性：宽高、像素格式（需要与Java层的格式一致）
                ANativeWindow_setBuffersGeometry(pWindow, pCodecCtx->width, pCodecCtx->height,
                                                 WINDOW_FORMAT_RGBA_8888);
                ANativeWindow_lock(pWindow, &out_buffer, NULL);

                //2、fix buffer

                //初始化缓冲区
                //设置属性，像素格式、宽高
                //rgb_frame的缓冲区就是Window的缓冲区，同一个，解锁的时候就会进行绘制
                avpicture_fill((AVPicture *) rgb_frame, out_buffer.bits, AV_PIX_FMT_RGBA,
                               pCodecCtx->width,
                               pCodecCtx->height);

                //YUV格式的数据转换成RGBA 8888格式的数据
                //FFmpeg可以转，但是会有问题，因此我们使用libyuv这个库来做
                //https://chromium.googlesource.com/external/libyuv
                //参数分别是数据、对应一行的大小
                //I420ToARGB(yuv_frame->data[0], yuv_frame->linesize[0],
                //           yuv_frame->data[1], yuv_frame->linesize[1],
                //           yuv_frame->data[2], yuv_frame->linesize[2],
                //           rgb_frame->data[0], rgb_frame->linesize[0],
                //           pCodecCtx->width, pCodecCtx->height);

                I420ToARGB(yuv_frame->data[0], yuv_frame->linesize[0],
                           yuv_frame->data[2], yuv_frame->linesize[2],
                           yuv_frame->data[1], yuv_frame->linesize[1],
                           rgb_frame->data[0], rgb_frame->linesize[0],
                           pCodecCtx->width, pCodecCtx->height);

                //3、unlock window
                ANativeWindow_unlockAndPost(pWindow);

                frame_count++;
                LOGI("解码绘制第%d帧", frame_count);
            }
        }

        //释放资源
        av_free_packet(packet);
    }

    av_frame_free(&yuv_frame);
    avcodec_close(pCodecCtx);
    avformat_free_context(pFormatCtx);
    (*env)->ReleaseStringUTFChars(env, input_, input);
}

