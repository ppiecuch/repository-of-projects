package com.demo.list.examples;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class Example00 extends Activity implements Interface {

	public native void demo();

	public void test() {
		demo();
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		TextView tv = new TextView(this);
		tv.setText("Example00");
		setContentView(tv);
		
		test();
	}
	
	static{
		System.loadLibrary("demo00");
	}

}
