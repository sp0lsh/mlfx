#include "EngineContext.h"
#include "Log.h"

EngineContext::jni_fields_t EngineContext::jni_fields = {nullptr};

EngineContext::EngineContext(int type) {
  m_pEngine = std::make_unique<Engine>();
}

EngineContext::~EngineContext() = default;

void EngineContext::init(ANativeWindow* window, size_t width, size_t height) {
  m_pEngine->init(window, width, height);
}

void EngineContext::render() {
  m_pEngine->render();
}

void EngineContext::draw(uint8_t* buffer,
                         size_t length,
                         size_t width,
                         size_t height,
                         float rotation) {
  m_pEngine->draw(buffer, length, width, height, rotation);
}

void EngineContext::createContext(JNIEnv* env, jobject obj, jint type) {
  auto* context = new EngineContext(type);

  storeContext(env, obj, context);
}

void EngineContext::storeContext(JNIEnv* env,
                                 jobject obj,
                                 EngineContext* context) {
  // Get a reference to this object's class
  jclass cls = env->GetObjectClass(obj);

  if (nullptr == cls) {
    LOGE("Could not find com/media/camera/preview/render/VideoRenderer.");
    return;
  }

  // Get the Field ID of the "mNativeContext" variable
  jni_fields.context = env->GetFieldID(cls, "mNativeContext", "J");
  if (nullptr == jni_fields.context) {
    LOGE("Could not find mNativeContext.");
    return;
  }

  env->SetLongField(obj, jni_fields.context, (jlong)context);
}

void EngineContext::deleteContext(JNIEnv* env, jobject obj) {
  if (nullptr == jni_fields.context) {
    LOGE("Could not find mNativeContext.");
    return;
  }

  auto* context = reinterpret_cast<EngineContext*>(
      env->GetLongField(obj, jni_fields.context));

  delete context;

  env->SetLongField(obj, jni_fields.context, 0L);
}

EngineContext* EngineContext::getContext(JNIEnv* env, jobject obj) {
  if (nullptr == jni_fields.context) {
    LOGE("Could not find mNativeContext.");
    return nullptr;
  }

  auto* context = reinterpret_cast<EngineContext*>(
      env->GetLongField(obj, jni_fields.context));

  return context;
}
