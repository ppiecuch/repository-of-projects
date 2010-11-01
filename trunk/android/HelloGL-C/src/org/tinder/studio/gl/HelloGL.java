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
	 * 注意：EGL Context Lost
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
		 * 当任何时候调用一个画图方法的时候。即所有的绘图操作都在onDrawFrame方法中进行，
		 * 在绘图之前，需要将屏幕清除成前面所指定的颜色，   
		 * 清除深度缓存，并且重置场景，然后就可以进行绘图了！ 
		 */
		@Override
		public void onDrawFrame(GL10 gl) {
			nativeRender();
			Log.d("HelloGLRender","drawFrame");
		}

		/*
		 * surface发生改变的时候调用，例如从竖屏切换到横屏的时候。当窗口的大小发生改变的时候，
		 * 当然，不管窗口的大小是否已经改变。   
		 * 它在程序开始时至少运行一次，所以我们在该方法中设置OpenGL场景的大小  
		 */
		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			nativeResize(width,height);
		}

		/*
		 * 当窗口创建的时候需要调用onSurfaceCreated，所以我们可以在里面对OpenGL做一些初始化的工作 
		 */
		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			nativeInit();
		}
		
	}
	
}