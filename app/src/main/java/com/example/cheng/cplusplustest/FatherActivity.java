/*
 * FatherActivity    2017-03-02
 * Copyright(c) 2017 Chengguo Co.Ltd. All right reserved.
 *
 */
package com.example.cheng.cplusplustest;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

/**
 * class description here
 *
 * @author cheng
 * @version 1.0.0
 * @since 2017-03-02
 */
public class FatherActivity extends Activity {

    public static Intent newInstance(Activity activity) {
        Intent intent = new Intent(activity, FatherActivity.class);

        return intent;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
}