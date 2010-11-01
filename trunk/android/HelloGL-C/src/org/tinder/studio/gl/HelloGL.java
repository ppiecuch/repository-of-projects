package org.tinder.studio.gl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;

public class HelloGL extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new HelloGLView(this));
    }
    
    static {
    	System.loadLibrary("hellogl");
    }


}
class HelloGLView extends GLSurfaceView{
	
	HelloGLRender renderer;

	public HelloGLView(Context context) {
		super(context);
		renderer=new HelloGLRender();
		setRenderer(renderer);
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
	class HelloGLRender implements GLSurfaceView.Renderer{
		
		private native void nativeInit();
	    private native void nativeResize(int width, int height);
	    private native void nativeRender();
	    private native void nativeDone();

		/*
		 * ���κ�ʱ�����һ����ͼ������ʱ�򡣼����еĻ�ͼ��������onDrawFrame�����н��У�
		 * �ڻ�ͼ֮ǰ����Ҫ����Ļ�����ǰ����ָ������ɫ��   
		 * �����Ȼ��棬�������ó�����Ȼ��Ϳ��Խ��л�ͼ�ˣ� 
		 */
		@Override
		public void onDrawFrame(GL10 gl) {
			nativeRender();
			Log.d("HelloGLRender","drawFrame");
		}

		/*
		 * surface�����ı��ʱ����ã�����������л���������ʱ�򡣵����ڵĴ�С�����ı��ʱ��
		 * ��Ȼ�����ܴ��ڵĴ�С�Ƿ��Ѿ��ı䡣   
		 * ���ڳ���ʼʱ��������һ�Σ����������ڸ÷���������OpenGL�����Ĵ�С  
		 */
		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			nativeResize(width,height);
		}

		/*
		 * �����ڴ�����ʱ����Ҫ����onSurfaceCreated���������ǿ����������OpenGL��һЩ��ʼ���Ĺ��� 
		 */
		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			nativeInit();
		}
		
	}
	
}