#配置CMake的最低版本
cmake_minimum_required(VERSION 3.4.1)

#配置工程路径
set(path_project /Users/georgeRen/AndroidStudioProjects/MyNDK2)

#配置头文件的包含路径
include_directories(${path_project}/app/src/main/cpp/include/ffmpeg)
include_directories(${path_project}/app/src/main/cpp/include/yuv)

#添加自己写的库
add_library(
             native-lib
             SHARED
             src/main/cpp/native-lib.c)

#添加FFmpeg预编译的so库
add_library( avutil
             SHARED
             IMPORTED
            )

#设置两个预编译的库的路径
set_target_properties(
                       avutil
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libavutil-54.so
                    )

add_library( swresample
             SHARED
             IMPORTED
            )

set_target_properties(
                       swresample
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libswresample-1.so
                    )

add_library( avcodec
             SHARED
             IMPORTED
            )

set_target_properties(
                       avcodec
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libavcodec-56.so
                    )

add_library( avformat
             SHARED
             IMPORTED
            )

set_target_properties(
                       avformat
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libavformat-56.so
                    )

add_library( swscale
             SHARED
             IMPORTED
            )

set_target_properties(
                       swscale
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libswscale-3.so
                    )

add_library( postproc
             SHARED
             IMPORTED
            )

set_target_properties(
                       postproc
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libpostproc-53.so
                    )

add_library( avfilter
             SHARED
             IMPORTED
            )

set_target_properties(
                       avfilter
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libavfilter-5.so
                    )

add_library( avdevice
             SHARED
             IMPORTED
            )

set_target_properties(
                       avdevice
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libavdevice-56.so
                    )

#YUV转RGB需要的库

add_library( yuv
             SHARED
             IMPORTED
            )

set_target_properties(
                       yuv
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libyuv.so
                    )

#找到Android的log库（这个库已经在Android平台中了）
find_library(
            log-lib
            log
            )

#找到Android的Window绘制相关的库（这个库已经在Android平台中了）
find_library(
            android-lib
            android
            )

#把需要的库链接到自己的库中
target_link_libraries(
            native-lib
            ${log-lib}
            ${android-lib}
            avutil
            swresample
            avcodec
            avformat
            swscale
            postproc
            avfilter
            avdevice
            yuv
            )