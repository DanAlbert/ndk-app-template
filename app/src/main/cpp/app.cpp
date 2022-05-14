#include <jni.h>

#include <string>

extern "C" JNIEXPORT jstring JNICALL string_from_jni(JNIEnv *env, jobject) {
  std::string hello = "Hello from C++";
  return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved __unused) {
  JNIEnv *env;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }

  jclass c = env->FindClass("com/example/myapplication/MainActivity");
  if (c == nullptr) return JNI_ERR;

  // Register your class' native methods.
  static const JNINativeMethod methods[] = {
      {"stringFromJNI", "()Ljava/lang/String;",
       reinterpret_cast<void *>(string_from_jni)},
  };
  int rc = env->RegisterNatives(c, methods,
                                sizeof(methods) / sizeof(JNINativeMethod));
  if (rc != JNI_OK) return rc;

  return JNI_VERSION_1_6;
}