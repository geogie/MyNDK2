# MyNDK2
 1. 利用ffmpeg + surfaceView视频播放（没有声音）。
 2. 播放音频 或者 视频的音频（没有视频）。c中调用java的方法：com.georgeren.myndk2.VideoPlayer中的方法createAudioTrack
 3. VideoUtils 视频解码(decode)，音频解码(sound)。解码后文件变大了？
# 注意：别忘记加读写权限。不然...坑。
 1. JNI：（Java Native Interface）：Java调用C/C++，C/C++调用Java的一套API。
 2. NDK 调试 ndk-stack
 3. 注意我用的native-lib.c,默认是cpp,.c和.cpp的语法有差别，比如引入方法在cpp中用::和命名空间有关。
 
[参考](http://www.jianshu.com/p/10406b11552f)