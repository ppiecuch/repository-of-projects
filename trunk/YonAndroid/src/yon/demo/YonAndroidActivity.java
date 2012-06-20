package yon.demo;

import yon.AndroidGLView;
import yon.SysApplication;
import yon.util.Util;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;

public class YonAndroidActivity extends Activity {
    
	private final String TAG="YonAndroidActivity";
	AndroidGLView view;
	Handler handler;
	AlertDialog.Builder confirmDialog;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);  
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);  
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,  WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        
        initHandler();
        
        view=new AndroidGLView(this,handler);
        setContentView(view);
        SysApplication.getInstance().addActivity(this);
        
        Log.i(TAG,Util.getExternalStoragePath());
        Log.i(TAG,Util.getFormatSize(Util.getAvailableStore(Util.getExternalStoragePath())));
        Log.i(TAG,Util.getLanguage());
        
        if(Util.detectOpenGLES20(this)){
        	Log.i(TAG, "support opengles 2.0");
        }
        else{
        	Log.i(TAG, "unsupport opengles 2.0");
        }
        
       
    }
    
    public void initHandler(){
    	initConfirmDialog();
    	this.handler=new YonHandler() {
			
			@Override
			public void showWating() {
			}
			
			@Override
			public void showToast(String text) {
			}
			
			@Override
			public void showConfirm(String title,String content,String ok,String cancel) {
				//TODO setTitle....
				//TODO 添加lifecycle
				confirmDialog.show();
			}
			
			@Override
			public void setupInput() {
			}
			
			@Override
			public void hideWating() {
			}
			
			@Override
			public void completeInput(String text) {
			}
		};
    }
    
    public void initConfirmDialog(){
    	confirmDialog = new AlertDialog.Builder(YonAndroidActivity.this);
//    	confirmExit=new Dialog(this,R.style.Dialog_Fullscreen);
    	
    	confirmDialog.setTitle("确认提醒");
    	confirmDialog.setMessage("您确定要退出吗?");
		confirmDialog.setCancelable(false);//不能通过back键关闭对话框
		
		confirmDialog.setPositiveButton("确认",new DialogInterface.OnClickListener(){
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
			}
		});
		confirmDialog.setNegativeButton("取消",new DialogInterface.OnClickListener(){
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
			}
		});
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