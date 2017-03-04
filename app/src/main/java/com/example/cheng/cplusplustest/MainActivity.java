package com.example.cheng.cplusplustest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import java.util.Date;
import java.util.Random;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    public String key = "guo";

    public static int count = 1;

    public Human man = new Man();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI() + "\n" +
                getString2Fromc(20) + "\n" +
                accessField() + "\n");
        accessStaticField();
        accessMethod();
        //只能访问android手机的文件，不能访问电脑文件
//        accessStaticMethod();
        accessConstructor();

        man.sayHi();
        //有问题
//        accessNonvirtualMethod();

        String srt = chineaseChars("宋喆");
        Log.e("tag", "调用回来的汉字：" + srt);

        int[] array = {9, 100, 10, 37, 5, 10};
        giveArray(array);
        //排序
        for (int i : array) {
            Log.e("tag", "排序后i=" + i);
        }

        int[] array1 = getArray(10);
        for (int i : array1) {
            Log.e("tag", "从jni中获取回来的数组i=" + i);
        }

        Log.e("tag", "--------------2017/3/4 -----");
        createGlobalRef();
        Log.e("tag", "--2017/3/4 --getGlobalRef()---"+getGlobalRef());
        //用完后释放
        deleteGlobalRef();

        try {
            //jni里面的异常
            exeception();
        }catch (Exception e){
            Log.e("tag", "---2017/3/4 --发生异常");
        }
        Log.e("tag", "---2017/3/4 -------发生异常之后----------");
        try {
            //jni里面的异常
            exeception();
        }catch (Exception e){
            Log.e("tag", "---2017/3/4 --发生异常");
        }

        //不断调用cacked方法
        for (int i = 0; i < 100; i++) {
            cached();
        }

    }

    //产生指定范围随机数
    public int getRandomInt(int max) {
        int i = new Random().nextInt(max);
        Log.e("tag", "getgetRandomInt 执行了...i=" + i);
        return i;
    }

    //长生UUID
    public static String getUUID() {
        return UUID.randomUUID().toString();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String getString2Fromc(int i);

    public native String accessField();

    public native void accessStaticField();

    public native void accessMethod();

    public native void accessStaticMethod();

    public native Date accessConstructor();

    public native void accessNonvirtualMethod();

    public native String chineaseChars(String in);

    public native void giveArray(int[] array);

    public native int[] getArray(int len);

    public native void localRef();

    public native void createGlobalRef();

    public native String getGlobalRef();

    public native void deleteGlobalRef();

    public native void exeception();

    public native static void initIds();



    public native void cached();
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");

        initIds();
    }
}
