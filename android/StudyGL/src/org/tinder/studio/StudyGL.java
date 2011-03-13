package org.tinder.studio;


import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

public class StudyGL extends Activity {
	
	AndroidGLView view;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        view=new AndroidGLView(this);
		setContentView(view);
    }
    
    @Override
    protected void onDestroy() {
		Log.d("StudyGL","Destroy");
    	super.onDestroy();
    }
	
	@Override
	protected void onPause() {
		Log.d("StudyGL","Pause");
		super.onPause();
		view.onPause();
	}
	
	@Override
	protected void onResume() {
		Log.d("StudyGL","Resume");
		super.onResume();
		view.onResume();
	}
    
    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		//按下的如果是BACK，同时没有重复
//		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { 
//	       return view.onBackDown();
//	       return true;
//	    }
	    return super.onKeyDown(keyCode, event);
	}
}