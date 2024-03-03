#include "EngineJNI.h"
#include "EngineContext.h"

#include <android/native_window_jni.h>

JCMCPRV(void, create)(JNIEnv* env, jobject obj, jint type) {
  EngineContext::createContext(env, obj, type);
}

JCMCPRV(void, destroy)(JNIEnv* env, jobject obj) {
  EngineContext::deleteContext(env, obj);
}

JCMCPRV(void, init)
(JNIEnv* env, jobject obj, jobject surface, jint width, jint height) {
  EngineContext* context = EngineContext::getContext(env, obj);

  ANativeWindow* window =
      surface ? ANativeWindow_fromSurface(env, surface) : nullptr;

  if (context)
    context->init(window, (size_t)width, (size_t)height);
}

JCMCPRV(void, render)(JNIEnv* env, jobject obj) {
  EngineContext* context = EngineContext::getContext(env, obj);

  if (context)
    context->render();
}

JCMCPRV(void, draw)
(JNIEnv* env,
 jobject obj,
 jbyteArray data,
 jint width,
 jint height,
 jint rotation) {
  jbyte* bufferPtr = env->GetByteArrayElements(data, nullptr);

  jsize arrayLength = env->GetArrayLength(data);

  EngineContext* context = EngineContext::getContext(env, obj);

  if (context)
    context->draw((uint8_t*)bufferPtr, (size_t)arrayLength, (size_t)width,
                  (size_t)height, rotation);

  env->ReleaseByteArrayElements(data, bufferPtr, 0);
}
