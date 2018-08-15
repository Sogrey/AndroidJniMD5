---
title: JNI Md5加密生成so文件
date: 2018-07-25 20:39:13
tags: [JNI,Md5]
categories: JNI
comments: true
toc: true
---

JNI Md5加密生成so文件

IDE工具:Android Studio
编译环境:JDK1.8+Android SDK+NDK

## 1 新建项目
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/新建项目-Android-jni-md5.jpg)

选中“Include C++ support”,一直“Next”.
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/选择c++Support.jpg)

等待项目构建完成后
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/项目目录结构.jpg)

看到 `java`包同级有个`cpp`的包就是存放c++代码的。

而里`native-lib.cpp`的代码很简单，就是打印出一句“Hello from C++”:
``` C++
#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring

JNICALL
Java_sogrey_android_1jni_1md5_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
```
运行一下：
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180803161827.jpg)

这就表示我们的C++环境正常，可以开始写我们自己的代码了。

## 2 新建Jni工具类
新建一个`JNIUtils.java`
``` java
package sogrey.android_jni_md5;

/**
 * Created by Sogrey on 2018/8/3.
 */

public class JNIUtils {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
```
把`MainActivity.java`中的相关代码删掉
> tv.setText(stringFromJNI());

也改成

> tv.setText(new JNIUtils().stringFromJNI());

## 3 生成.h文件,编写cpp文件
Android studio的Terminal中依次执行命令：
> cd app/src/main/java
> javah -jni sogrey.android_jni_md5.JNIUtils

![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM—20180803164015.jpg)

将生成的`sogrey_android_jni_md5_JNIUtils.h`头文件剪贴到`cpp`包下，并复制一份重命名为`sogrey_android_jni_md5_JNIUtils.cpp`.
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180803164408.jpg)

编辑复制的`.cpp`文件，删掉多余代码只留下需要我们实现的那个方法,加上返回方法体，并导入之前的`.h`文件
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180803164549.jpg)

``` c++
#include "sogrey_android_jni_md5_JNIUtils.h"
/*
 * Class:     sogrey_android_jni_md5_JNIUtils
 * Method:    stringFromJNI
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_sogrey_android_1jni_1md5_JNIUtils_stringFromJNI
        (JNIEnv *env, jobject obj){
}
```
方法需要返回`jstring`,可以参照之前`native-lib.cpp`里的代码实现，引入提示需要导入`string`:
``` c++
#include <string>
```
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM截图20180803165042.jpg)

则最终代码就是：
``` c++
#include <string>
#include "sogrey_android_jni_md5_JNIUtils.h"
/*
 * Class:     sogrey_android_jni_md5_JNIUtils
 * Method:    stringFromJNI
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_sogrey_android_1jni_1md5_JNIUtils_stringFromJNI
        (JNIEnv *env, jobject obj){
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
```
## 4 修改`CMakeLists.txt`文件
我们刚刚添加了一个新的c++文件，现将它加到CMakeLists中，刚才的`native-lib.cpp`已经没有用了，注掉它
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180803165959.jpg)

重新编译运行看会不会得到和之前一样的结果。（结果是一样的）
在`build`>`intermediates`>`cmake`>`debug`>`obj`下就能找到生成的.so文件了
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180803170704.jpg)

## 5 添加`getMd5(String)`的native方法
在`JNIUtils.java`中添加下面native方法：
``` java
    /**
     * 获取指定字符串md5值
     * @param content 要md5加密的字符串
     * @return md5加密串
     */
    public native String getMd5(String content);
```
重复第三步操作。或者熟练了直接在`sogrey_android_jni_md5_JNIUtils.h`文件中添加：
``` c++
/*
 * Class:     sogrey_android_jni_md5_JNIUtils
 * Method:    getMd5
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_sogrey_android_1jni_1md5_JNIUtils_getMd5
        (JNIEnv *, jobject, jstring);
```
同样，需要在`sogrey_android_jni_md5_JNIUtils.cpp`中去实现它：
``` c++
/*
 * Class:     sogrey_android_jni_md5_JNIUtils
 * Method:    getMd5
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_sogrey_android_1jni_1md5_JNIUtils_getMd5
        (JNIEnv *env, jobject obj, jstring str){
    //TODO 实现方法逻辑
}
```
## 6 实现MD5加密
md5的加密方法网上很多，这里给出一个c++实现的[[MD5.cpp](https://github.com/Sogrey/AndroidJniMD5/blob/master/app/src/main/cpp/MD5.cpp),[MD5.h](https://github.com/Sogrey/AndroidJniMD5/blob/master/app/src/main/cpp/MD5.h)].直接将两个文件引入到cpp包下，
在`sogrey_android_jni_md5_JNIUtils.cpp`引入
> #include "MD5.h"

则`getMD5`方法实现：
``` c++
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
```

其中：JNI_FALSE 是个宏定义表示 jboolean false
``` c++
#define JNI_FALSE 0
#define JNI_TRUE 1
```

运行一把，得到md5字符串
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180803225817.jpg)

## 7 生成.so文件

在cmake.txt中添加

    #设置生成的so动态库最后输出的路径
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

用来指定生成的so文件存储路径，点击`build`>`Make project`,看到在`jniLibs`文件夹下生成支持4中不同内核的so文件。
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180804002050.jpg)

想要配置指定内核平台，修改`Module`下的`build.gradle`中的`android`>`defaultConfig` 添加：
```
externalNativeBuild {
            cmake {
                cppFlags ""
                abiFilters "arm64-v8a","armeabi-v7a","x86","x86_64"
//Error:ABIs [armeabi] are not supported for platform. Supported ABIs are [armeabi-v7a, arm64-v8a, x86, x86_64].
            }
        }
```

## 8 修改库名称
默认生成的so库名称是`native-lib`,怎么修改为自定义的名称呢。

首先，修改`CMakeLists.txt`内容：修改`add_library`和`target_link_libraries`第一个参数为我们想自定定义的名称，比如`md5Lib`
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180804003217.jpg)

同样需要修改`JNIUtils.java`中`System.loadLibrary(String)`的参数为上面我们自定义的名称`md5Lib`
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180804003613.jpg)

重新点击`build`>`Make project`,看到在`jniLibs`文件夹下生成新的so文件。
![](https://raw.githubusercontent.com/Sogrey/AndroidJniMD5/master/screenshot/TIM-20180804004506.jpg)

项目源码地址[github](https://github.com/Sogrey/AndroidJniMD5)

