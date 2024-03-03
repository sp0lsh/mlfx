#ifndef _H_MLFX_PLATFORM_
#define _H_MLFX_PLATFORM_

#ifdef __ANDROID__

  #include <android/log.h>

  #define DEBUG 1

  #define LOG_TAG "media-lib"
  #if DEBUG
    #define DBG_LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define DBG_LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define DBG_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)    
  #else
    #define DBG_LOG(...)
    #define DBG_LOGI(...)
    #define DBG_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)    
  #endif

  #include <android/native_window.h>
  #define MLFX_NativeWindow ANativeWindow

  #include <GLES3/gl3.h>

#elif defined(__APPLE__) && defined(__MACH__)
  // Apple OSX and iOS (Darwin).
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR == 1
  // iOS in Xcode simulator

  #elif TARGET_OS_IPHONE == 1
  // iOS

  #elif TARGET_OS_MAC == 1
    // macOS

    #define DBG_LOG(...) printf(__VA_ARGS__)
    #define DBG_LOGI(...) printf(__VA_ARGS__)
    #define DBG_LOGE(...) printf(__VA_ARGS__)

    #define MLFX_NativeWindow void

    #include <angle_gl.h>

  #endif // TARGET OS

#else // PLATFORM
  #error "Uknown platform"
#endif // PLATFORM

#endif // _H_MLFX_PLATFORM_

