package yon.demo;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import org.fmod.FMODAudioDevice;

import yon.AndroidGLView;
import yon.SysApplication;
import yon.input.MyEditText;
import yon.input.MyEditorActionListener;
import yon.util.Util;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.Toast;

public class YonAndroidActivity extends Activity{

	private final String TAG = "YonAndroidActivity";
	AndroidGLView view;
	Handler handler;
	ConfirmDialog confirmDialog;
	ProgressDialog spinnerDialog;
	SoftInputPanel inputPanel;

	MyEditText editText;
	MyEditorActionListener listener;
	InputMethodManager imm;

	FMODAudioDevice mFMODAudioDevice = new FMODAudioDevice();
	
	private void toast(String s) {
    	Log.d(TAG,s);
        Context context = getApplicationContext();
        int duration = Toast.LENGTH_SHORT;
        Toast toast = Toast.makeText(context, s, duration);
        toast.show();
    }
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		toast("onConfigurationChanged");
		super.onConfigurationChanged(newConfig);
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		toast("onCreate");
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		//Òþ²Ø×´Ì¬À¸Í¼±ê
		if(Util.supportsSystemUiFlags())
			getWindow().getDecorView().setSystemUiVisibility(View.STATUS_BAR_HIDDEN);
		
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		initHandler();

		view = new AndroidGLView(this, handler);
		setContentView(view);
		SysApplication.getInstance().addActivity(this);

		editText = new MyEditText(this);
		editText.setFocusableInTouchMode(true);
		listener = new MyEditorActionListener(editText, handler);
		editText.setOnEditorActionListener(listener);
		imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		ViewGroup.LayoutParams layoutParams = new ViewGroup.LayoutParams(-1, -1);
		addContentView(editText, layoutParams);
		editText.setVisibility(View.GONE);

		initSpinner();
		initConfirmDialog();
		initInputPanel();

	}
	
	/*
	private void hideStatusBar(){
		
		Object object = getSystemService("statusbar");

	    Class<?> statusBarManager;
	    try {
	        statusBarManager = Class.forName("android.app.StatusBarManager");

	        Field field = statusBarManager.getDeclaredField("DISABLE_MASK");

	        Method disable = statusBarManager.getMethod("disable",new Class[] {int.class});
	        disable.setAccessible(true);
	        field.setAccessible(true);
	        
	        disable.invoke(object,field.getInt(object));

	    } catch (ClassNotFoundException e) {
	        logger.warn(Util.expandException(e));
	    } catch (Exception e) {
	    	logger.warn(Util.expandException(e));
	    }
	}*/

	public void initHandler() {
		this.handler = new YonHandler() {

			@Override
			public void showWating(String text) {
				spinnerDialog.setMessage(text);
				spinnerDialog.show();
			}

			@Override
			public void hideWating() {
				spinnerDialog.hide();
			}

			@Override
			public void showToast(String text) {
				Toast toast = new Toast(YonAndroidActivity.this);
				TextView tv = new TextView(YonAndroidActivity.this);
				tv.setText(text);
				toast.setView(tv);
				toast.show();
			}

			@Override
			public void showConfirm(String title, String content, String ok,
					String cancel) {
				// TODO Ìí¼Ólifecycle
				confirmDialog.setTitle(title);
				confirmDialog.setMessage(content);
				confirmDialog.setPositiveString(ok);
				confirmDialog.setNegativeString(cancel);
				confirmDialog.show();
			}

			@Override
			public void setupInput() {
				// inputPanel.setVisible(true);
				editText.setVisibility(View.VISIBLE);
				editText.setFocusable(true);
				editText.requestFocus();
				editText.setText("");
				imm.showSoftInput(editText,
						InputMethodManager.RESULT_UNCHANGED_SHOWN);
				// Log.i("handler","setupInput");
			}

			@Override
			public void completeInput(String text) {
				editText.clearFocus();
				editText.setVisibility(View.GONE);
				imm.hideSoftInputFromInputMethod(editText.getWindowToken(),
						InputMethodManager.RESULT_HIDDEN);
				String[] args = new String[1];
				args[0] = text;
				view.nativeOnUI(Constant.MSG_COMPLETE_INPUT, args);
				// Log.i("handler",text);
			}
		};
	}

	public void initSpinner() {
		spinnerDialog = new ProgressDialog(this);
		spinnerDialog.setIndeterminate(true);
		spinnerDialog.setCancelable(true);
	}

	public void initConfirmDialog() {
		confirmDialog = new ConfirmDialog(this) {

			@Override
			public boolean onClickPositive() {
				String[] strs = new String[] { "²âÊÔ" };
				return view.nativeOnUI(Constant.MSG_POSITIVE_CONFIRM, strs);
			}

			@Override
			public boolean onClickNegative() {
				return view.nativeOnUI(Constant.MSG_NEGATIVE_CONFIRM, null);
			}
		};
	}

	public void initInputPanel() {
		ViewGroup.LayoutParams layoutParams = new ViewGroup.LayoutParams(400,
				300);
		inputPanel = new SoftInputPanel(this) {

			@Override
			public void completeInput(String text) {
				String[] strs = new String[1];
				strs[0] = text;
				view.nativeOnUI(Constant.MSG_COMPLETE_INPUT, strs);
			}
		};
		addContentView(inputPanel.getView(), layoutParams);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return view.onKeyDown(keyCode, event);
	}
	
	@Override
	protected void onRestart() {
		toast("onRestart");
		super.onRestart();
	}

	@Override
	public void onStart() {
		toast("onStart");
		super.onStart();
		mFMODAudioDevice.start();
	}

	@Override
	public void onStop() {
		toast("onStop");
		mFMODAudioDevice.stop();
		super.onStop();
	}

	static {
		System.loadLibrary("fmodex");
	}

	@Override
	protected void onPause() {
		toast("onPause");
		view.onPause();
		super.onPause();
	}

	@Override
	protected void onResume() {
		toast("onResume");
		super.onResume();
		view.onResume();
	}

	@Override
	protected void onDestroy() {
		toast("onDestroy");
		// view.onDestroy();
		super.onDestroy();
		//SysApplication.getInstance().exit();
	}
}