package org.tinder.studio;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class AndroidGLView extends GLSurfaceView{
	
	AndroidGLRender renderer;
	Activity context;
	
	private native void nativeInit(String apkFilePath);
	private native void nativeResize(int width, int height);
	private native void nativeRender();
	private native void nativePress(float x,float y);
	private native void nativeRelease(float x,float y);
	private native void nativeDone();
	private native boolean nativeOnBackDown();
	
	private void returnToMainMenu(){
		context.finish();
	}
	private void openPreference(){
		
	}

	public AndroidGLView(Activity context) {
		super(context);
		this.context=context;
		renderer=new AndroidGLRender();
		setRenderer(renderer);
	}
	
	public boolean onTouchEvent(final MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			nativePress(event.getX(),event.getY());
		}
		else if(event.getAction() == MotionEvent.ACTION_UP){
			nativeRelease(event.getX(),event.getY());
		}
		return true;
	}
	
	public boolean onBackDown(){
		return nativeOnBackDown();
	}

	
	/*
	 * ע�⣺EGL Context Lost
	 * There are situations where the EGL rendering context will be lost. 
	 * This typically happens when device wakes up after going to sleep. 
	 * When the EGL context is lost, all OpenGL resources (such as textures) that
	 *  are associated with that context will be automatically deleted. 
	 *  In order to keep rendering correctly, a renderer must recreate any lost resources that it still needs. 
	 *  The onSurfaceCreated(GL10, EGLConfig) method is a convenient place to do this.
	 */
	class AndroidGLRender implements GLSurfaceView.Renderer{

		/*
		 * ���κ�ʱ�����һ����ͼ������ʱ�򡣼����еĻ�ͼ��������onDrawFrame�����н��У�
		 * �ڻ�ͼ֮ǰ����Ҫ����Ļ�����ǰ����ָ������ɫ��   
		 * �����Ȼ��棬�������ó�����Ȼ��Ϳ��Խ��л�ͼ�ˣ� 
		 */
		public void onDrawFrame(GL10 gl) {
			nativeRender();
		}

		/*
		 * surface�����ı��ʱ����ã�����������л���������ʱ�򡣵����ڵĴ�С�����ı��ʱ��
		 * ��Ȼ�����ܴ��ڵĴ�С�Ƿ��Ѿ��ı䡣   
		 * ���ڳ���ʼʱ��������һ�Σ����������ڸ÷���������OpenGL�����Ĵ�С  
		 */
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			nativeResize(width,height);
		}

		/*
		 * �����ڴ�����ʱ����Ҫ����onSurfaceCreated���������ǿ����������OpenGL��һЩ��ʼ���Ĺ��� 
		 */
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			String apkFilePath = null;
			ApplicationInfo appInfo = null;
			PackageManager packMgmr = getContext().getPackageManager();
			try {
			    appInfo = packMgmr.getApplicationInfo("org.tinder.studio", 0);
		    } catch (NameNotFoundException e) {
			    e.printStackTrace();
			    throw new RuntimeException("Unable to locate assets, aborting...");
		    }
			apkFilePath = appInfo.sourceDir;
			nativeInit(apkFilePath);
		}
		
	}
	
	static {
    	System.loadLibrary("studygl");
    }
	
}
