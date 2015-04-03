package com.example.jafdemo;

import org.androidannotations.annotations.Click;
import org.androidannotations.annotations.EActivity;
import org.androidannotations.annotations.LongClick;
import org.androidannotations.annotations.ViewById;

import android.app.Activity;
import android.content.Intent;
import android.widget.EditText;
import android.widget.TextView;

@EActivity(R.layout.activity_main)
public class MainActivity extends Activity {

    @ViewById(R.id.txt)
    TextView txt;

    @Click
    void txt() {
         txt.setText("Hello gogogo...");
    }
    
    @Click
	void clickhere() {
    	startActivity(new Intent(this, TestActivity_.class));
	}
}