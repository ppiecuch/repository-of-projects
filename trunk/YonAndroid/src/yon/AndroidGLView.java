package yon;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import yon.util.Util;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class AndroidGLView extends GLSurfaceView{
	
	AndroidGLRender renderer;
	Activity activity;
	int screenWidth,screenHeight;
	
	private native void nativeOnSurfaceCreated(int width,int height,String apkFilePath,String sdcardPath);
	private native void nativeOnSurfaceChanged(int width, int height);
	private native void nativeOnDrawFrame();
	private native void nativeOnPause();
	private native void nativeOnResume();
	private native boolean nativeOnBack();
	private native boolean nativeOnTouch(int action,float[] xs,float[] ys,int count);
	
	private void callbackDestroy(){
		activity.finish();
	}
	

	public AndroidGLView(Context context) {
		super(context);
		this.activity=(Activity)context;
		
		DisplayMetrics dm = new DisplayMetrics();
		activity.getWindowManager().getDefaultDisplay().getMetrics(dm);
		screenWidth = dm.widthPixels;
		screenHeight = dm.heightPixels;
		
		renderer=new AndroidGLRender();
		setRenderer(renderer);
	}
	
	@Override
	public void onPause() {
		//Log.d("AndroidGLView","onPause");
		super.onPause();
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { 
			Log.d("AndroidGLView","onBack");
			return nativeOnBack();
			//activity.finish();
		}
		return super.onKeyDown(keyCode, event);
	}
	
	public boolean onTouchEvent(final MotionEvent event) {
		//long start=SystemClock.uptimeMillis();
		switch (event.getAction() & MotionEvent.ACTION_MASK) 
		{
        case MotionEvent.ACTION_DOWN:
        case MotionEvent.ACTION_UP:
        case MotionEvent.ACTION_POINTER_DOWN:
        case MotionEvent.ACTION_POINTER_UP:
        case MotionEvent.ACTION_MOVE:
		}
		int count=event.getPointerCount();
    	float xs[]=new float[count];
    	float ys[]=new float[count];
    	for(int i=0;i<count;++i)
    	{
    		xs[i]=event.getX(i);
    		ys[i]=event.getY(i);
    	}
		//nativeOnTouch(event.getAction(),event.getX(),event.getY());
    	nativeOnTouch(event.getAction(),xs,ys,count);
		//Log.d("AndroidGLView","use:"+(SystemClock.uptimeMillis()-start)+"ms");
		//����reture true,����up/move�¼��޷���ץ
		return true;
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

		public void onDrawFrame(GL10 gl) {
			nativeOnDrawFrame();
		}

		/*
		 * surface�����ı��ʱ����ã�����������л���������ʱ�򡣵����ڵĴ�С�����ı��ʱ��
		 * ��Ȼ�����ܴ��ڵĴ�С�Ƿ��Ѿ��ı䡣   
		 * ���ڳ���ʼʱ��������һ�Σ����������ڸ÷���������OpenGL�����Ĵ�С  
		 */
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			nativeOnSurfaceChanged(width,height);
		}

		/*
		 * �����ڴ�����ʱ����Ҫ����onSurfaceCreated���������ǿ����������OpenGL��һЩ��ʼ���Ĺ��� 
		 */
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			nativeOnSurfaceCreated(screenWidth,screenHeight,Util.getAPKFilePath(activity),Util.getSdCardPath());
		}
		
	}
	
	static {
    	System.loadLibrary("yon");
    }
	
}