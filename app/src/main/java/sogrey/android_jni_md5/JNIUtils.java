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
    public native static String stringFromJNI();

    /**
     * 获取指定字符串md5值
     * @param content 要md5加密的字符串
     * @return md5加密串
     */
    public native static String getMd5(String content);
}
