package com.example.cheng.cplusplustest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import java.util.Random;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    public String key = "guo";

    public static int count = 1;

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
//        accessStaticMethod();
    }

    //产生指定范围随机数
    public int getRandomInt(int max) {
        int i = new Random().nextInt(max);
        Log.e("tag", "getgetRandomInt 执行了...i=" + i);
        return i;
    }

    //长生UUID
    public String getUUID(){
        return  UUID.randomUUID().toString();
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


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
