package org.tinder.studio.gl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class AndroidGLActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new AndroidGLView(this));
    }
}
class AndroidGLView extends GLSurfaceView{
	
	AndroidGLRender renderer;
	
	private native void nativeInit(String apkFilePath);
	private native void nativeResize(int width, int height);
	private native void nativeRender();
	private native void nativePause();
	private native void nativeDone();

	public AndroidGLView(Context context) {
		super(context);
		renderer=new AndroidGLRender();
		setRenderer(renderer);
	}
	
	public boolean onTouchEvent(final MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			nativePause();
		}
		return true;
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
	class AndroidGLRender implements GLSurfaceView.Renderer{

		/*
		 * 当任何时候调用一个画图方法的时候。即所有的绘图操作都在onDrawFrame方法中进行，
		 * 在绘图之前，需要将屏幕清除成前面所指定的颜色，   
		 * 清除深度缓存，并且重置场景，然后就可以进行绘图了！ 
		 */
		public void onDrawFrame(GL10 gl) {
			nativeRender();
//			Log.d("HelloGLRender","drawFrame");
		}

		/*
		 * surface发生改变的时候调用，例如从竖屏切换到横屏的时候。当窗口的大小发生改变的时候，
		 * 当然，不管窗口的大小是否已经改变。   
		 * 它在程序开始时至少运行一次，所以我们在该方法中设置OpenGL场景的大小  
		 */
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			nativeResize(width,height);
		}

		/*
		 * 当窗口创建的时候需要调用onSurfaceCreated，所以我们可以在里面对OpenGL做一些初始化的工作 
		 */
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			String apkFilePath = null;
			ApplicationInfo appInfo = null;
			PackageManager packMgmr = AndroidGLView.this.getContext().getPackageManager();
			try {
			    appInfo = packMgmr.getApplicationInfo("org.tinder.studio.gl", 0);
		    } catch (NameNotFoundException e) {
			    e.printStackTrace();
			    throw new RuntimeException("Unable to locate assets, aborting...");
		    }
			apkFilePath = appInfo.sourceDir;
			nativeInit(apkFilePath);
		}
		
	}
	
	static {
    	System.loadLibrary("androidgl");
    }
	
}