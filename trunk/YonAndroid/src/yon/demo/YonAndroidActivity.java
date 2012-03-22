package yon.demo;

import yon.AndroidGLView;
import yon.SysApplication;
import yon.util.Util;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;

public class YonAndroidActivity extends Activity {
    
	AndroidGLView view;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);  
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);  
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,  WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        view=new AndroidGLView(this);
        setContentView(view);
        SysApplication.getInstance().addActivity(this);
        
        if(Util.detectOpenGLES20(this)){
        	Log.i("XC2D", "support opengles 2.0");
        }
        else{
        	Log.i("XC2D", "unsupport opengles 2.0");
        }
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	return view.onKeyDown(keyCode, event);
    }
    
    @Override
    protected void onPause() {
    	super.onPause();
    	view.onPause();
    }
    
    @Override
	protected void onDestroy() {
		super.onDestroy();
		SysApplication.getInstance().exit();
		Log.i("XC2D", "exit");
	}
}