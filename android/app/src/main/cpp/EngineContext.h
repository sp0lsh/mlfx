#ifndef _H_ENGINE_CONTEXT_
#define _H_ENGINE_CONTEXT_

#include <mlfx/engine.h>

#include <jni.h>
#include <memory>

class EngineContext {
 public:
  struct jni_fields_t {
    jfieldID context;
  };

  EngineContext(int type);

  ~EngineContext();

  void init(ANativeWindow* window, size_t width, size_t height);

  void render();

  void draw(uint8_t* buffer,
            size_t length,
            size_t width,
            size_t height,
            float rotation);

  static void createContext(JNIEnv* env, jobject obj, jint type);

  static void storeContext(JNIEnv* env, jobject obj, EngineContext* context);

  static void deleteContext(JNIEnv* env, jobject obj);

  static EngineContext* getContext(JNIEnv* env, jobject obj);

 private:
  std::unique_ptr<Engine> m_pEngine;

  static jni_fields_t jni_fields;
};

#endif  // _H_ENGINE_CONTEXT_
