#include <string>
#include "sogrey_android_jni_md5_JNIUtils.h"
#include "MD5.h"

#define JNI_FALSE 0
#define JNI_TRUE 1

/*
 * Class:     sogrey_android_jni_md5_JNIUtils
 * Method:    stringFromJNI
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_sogrey_android_1jni_1md5_JNIUtils_stringFromJNI
        (JNIEnv *env, jobject obj) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

/*
 * Class:     sogrey_android_jni_md5_JNIUtils
 * Method:    getMd5
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_sogrey_android_1jni_1md5_JNIUtils_getMd5
        (JNIEnv *env, jobject obj, jstring str) {
    const char *originStr;
    //将jstring转化成char *类型
    originStr = env->GetStringUTFChars(str,JNI_FALSE);
    MD5 md5 = MD5(originStr);
    std::string md5Result = md5.hexdigest();
//    //将char *类型转化成jstring返回给Java层
    return env->NewStringUTF(md5Result.c_str());
}

