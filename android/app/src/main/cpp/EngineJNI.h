#ifndef _H_ENGINE_JNI_
#define _H_ENGINE_JNI_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JCMCPRV(rettype, name) \
  rettype JNIEXPORT JNICALL    \
      Java_com_media_camera_preview_render_VideoRenderer_##name

JCMCPRV(void, create)(JNIEnv* env, jobject obj, jint type);
JCMCPRV(void, destroy)(JNIEnv* env, jobject obj);
JCMCPRV(void, init)
(JNIEnv* env, jobject obj, jobject surface, jint width, jint height);
JCMCPRV(void, render)(JNIEnv* env, jobject obj);
JCMCPRV(void, draw)
(JNIEnv* env,
 jobject obj,
 jbyteArray data,
 jint width,
 jint height,
 jint rotation);

#ifdef __cplusplus
}
#endif

#endif  // _H_ENGINE_JNI_
