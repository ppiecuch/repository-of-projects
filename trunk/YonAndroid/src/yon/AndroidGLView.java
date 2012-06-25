package yon;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import yon.demo.Constant;
import yon.util.Util;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.InputType;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;

public class AndroidGLView extends GLSurfaceView{
	
	AndroidGLRender renderer;
	Activity activity;
	Handler handler;
	int screenWidth,screenHeight;
	MoveEvent currentMoveEvent,lastMoveEvent;
	
	protected boolean useSoftInput = false;
    protected boolean hideSoftInputWhenDone = true;
    protected TextInputListener textInputListener;
	
	private native void nativeOnSurfaceCreated(int width,int height,String apkFilePath,String sdcardPath);
	private native void nativeOnSurfaceChanged(int width, int height);
	private native void nativeOnDrawFrame();
	private native void nativeOnPause();
	private native void nativeOnResume();
	private native boolean nativeOnBack();
	private native boolean nativeOnTouch(int action,int id,float x,float y,int count);
	private native boolean nativeOnMove(int action,int ids[],float[] xs,float[] ys,int count);
	public native boolean nativeOnUI(int msg,String[] args);
	
	public native void nativeDebug(String str);
	public native void nativeInfo(String str);
	public native void nativeWarn(String str);
	public native void nativeError(String str);
	
	private void nativeCallback(int msg,String[] args){
		switch(msg)
		{
		case Constant.MSG_SETUP_CONFIRM:
		{
			if(args==null||args.length<4)
			{
				nativeError("cannot show confirm for unexpect args:"+Util.merge(args));
				return;
			}
			String title=args[0];
			String content=args[1];
			String ok=args[2];
			String cancel=args[3];
			Message message = handler.obtainMessage(Constant.MSG_SETUP_CONFIRM);
			Bundle bundle=new Bundle();
			bundle.putString(Constant.MSG_KEY_TITLE, title);
			bundle.putString(Constant.MSG_KEY_CONTENT, content);
			bundle.putString(Constant.MSG_KEY_POSITIVE_BUTTON, ok);
			bundle.putString(Constant.MSG_KEY_NEGATIVE_BUTTON, cancel);
			message.setData(bundle);
			handler.sendMessage(message);
			nativeDebug("receive callback confirm ui\r\n");
			break;
		}
		case Constant.MSG_TOAST:
		{
			if(args==null||args.length<1)
			{
				nativeError("cannot show toast for unexpect args:"+Util.merge(args));
				return;
			}
			String content=args[0];
			Message message = handler.obtainMessage(Constant.MSG_TOAST);
			Bundle bundle=new Bundle();
			bundle.putString(Constant.MSG_KEY_CONTENT, content);
			message.setData(bundle);
			handler.sendMessage(message);
			nativeDebug("receive callback toast ui\r\n");
			break;
		}
		case Constant.MSG_SHOW_SPINNER:
		{
			if(args==null||args.length<1)
			{
				nativeError("cannot show toast for unexpect args:"+Util.merge(args));
				return;
			}
			String content=args[0];
			Message message = handler.obtainMessage(Constant.MSG_SHOW_SPINNER);
			Bundle bundle=new Bundle();
			bundle.putString(Constant.MSG_KEY_CONTENT, content);
			message.setData(bundle);
			handler.sendMessage(message);
			nativeDebug("receive callback show spinner ui\r\n");
			break;
		}
		case Constant.MSG_HIDE_SPINNER:
		{
			handler.sendEmptyMessage(Constant.MSG_HIDE_SPINNER);
			nativeDebug("receive callback hide spinner ui\r\n");
			break;
		}
		case Constant.MSG_SETUP_INPUT:
		{
			//handler.sendEmptyMessage(Constant.MSG_SETUP_INPUT);
			showSoftInput();
			nativeDebug("receive callback show input panel ui\r\n");
			break;
		}
		default:
			nativeWarn("unexpect native callback!");
		}
	}
	
	private void callbackDestroy(){
		activity.finish();
	}
//	private void callbackShowConfirm(String title,String content,String ok,String cancel){
//		
//	}
	

	public AndroidGLView(Context context,Handler handler) {
		super(context);
		this.activity=(Activity)context;
		this.handler=handler;
		
		DisplayMetrics dm = new DisplayMetrics();
		activity.getWindowManager().getDefaultDisplay().getMetrics(dm);
		screenWidth = dm.widthPixels;
		screenHeight = dm.heightPixels;
		
		currentMoveEvent=new MoveEvent();
		lastMoveEvent=new MoveEvent();
		
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
	
	class MoveEvent{
		private static final int MAX_EVENT=16;
		public int count=-1;
		public int[] ids=new int[MAX_EVENT];
		public float[] xs=new float[MAX_EVENT];
		public float[] ys=new float[MAX_EVENT];
		
		private boolean equals(int[] a,int[] b)
		{
			/*if(a==null&&b==null)
				return true;
			if(a==null)
				return false;
			if(b==null)
				return false;
			if(a.length!=b.length)
				return false;*/
			for(int i=0;i<count;++i)
				if(a[i]!=b[i])
					return false;
			return true;
		}
		
		private boolean equals(float[] a,float[] b)
		{
			/*if(a==null&&b==null)
				return true;
			if(a==null)
				return false;
			if(b==null)
				return false;
			if(a.length!=b.length)
				return false;*/
			for(int i=0;i<count;++i)
				if(a[i]!=b[i])
					return false;
			return true;
		}
		
		public void set(MoveEvent e)
		{
			count=e.count;
			for(int i=0;i<count;++i)
			{
				ids[i]=e.ids[i];
				xs[i]=e.xs[i];
				ys[i]=e.ys[i];
			}
		}
		
		@Override
		public boolean equals(Object o) {
			if(o instanceof MoveEvent==false)
				return false;
			MoveEvent m=(MoveEvent)o;
			return m.count==count&&equals(ids, m.ids)&&equals(xs, m.xs)&&equals(ys, m.ys);
		}
	}
	
	public boolean onTouchEvent(final MotionEvent event) {
		//long start=SystemClock.uptimeMillis();
		final int id=(event.getAction()&MotionEvent.ACTION_POINTER_ID_MASK)>>MotionEvent.ACTION_POINTER_ID_SHIFT;
		currentMoveEvent.count=event.getPointerCount();
		switch (event.getAction() & MotionEvent.ACTION_MASK) 
		{
        case MotionEvent.ACTION_DOWN:
        case MotionEvent.ACTION_UP:
        case MotionEvent.ACTION_POINTER_DOWN:
        case MotionEvent.ACTION_POINTER_UP:
        	nativeOnTouch(event.getAction(),event.getPointerId(id),event.getX(id),event.getY(id),currentMoveEvent.count);
        	//未确定是否有必要使用queueEvent
        	/*
        	queueEvent(new Runnable() {
				@Override
				public void run() {
					nativeOnTouch(event.getAction(),event.getPointerId(id),event.getX(id),event.getY(id),currentMoveEvent.count);
				}
			});*/
        	return true;
        case MotionEvent.ACTION_MOVE:
        	for(int i=0;i<currentMoveEvent.count;++i)
        	{
        		currentMoveEvent.ids[i]=event.getPointerId(i);
        		currentMoveEvent.xs[i]=event.getX(i);
        		currentMoveEvent.ys[i]=event.getY(i);
        	}
        	if(lastMoveEvent.equals(currentMoveEvent))
        		return true;
        	lastMoveEvent.set(currentMoveEvent);
        	nativeOnMove(event.getAction(),currentMoveEvent.ids,currentMoveEvent.xs,currentMoveEvent.ys,currentMoveEvent.count);
        	/*queueEvent(new Runnable() {
				@Override
				public void run() {
					nativeOnMove(event.getAction(),currentMoveEvent.ids,currentMoveEvent.xs,currentMoveEvent.ys,currentMoveEvent.count);
				}
			});*/
        	return true;
		}
		//nativeOnTouch(event.getAction(),event.getX(),event.getY());
    	//nativeOnTouch(event.getAction(),xs,ys,count);
		//Log.d("AndroidGLView","use:"+(SystemClock.uptimeMillis()-start)+"ms");
		//必须reture true,否则up/move事件无法捕抓
		return false;
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

		public void onDrawFrame(GL10 gl) {
			nativeOnDrawFrame();
		}

		/*
		 * surface发生改变的时候调用，例如从竖屏切换到横屏的时候。当窗口的大小发生改变的时候，
		 * 当然，不管窗口的大小是否已经改变。   
		 * 它在程序开始时至少运行一次，所以我们在该方法中设置OpenGL场景的大小  
		 */
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			nativeOnSurfaceChanged(width,height);
		}

		/*
		 * 当窗口创建的时候需要调用onSurfaceCreated，所以我们可以在里面对OpenGL做一些初始化的工作 
		 */
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			nativeOnSurfaceCreated(screenWidth,screenHeight,Util.getAPKFilePath(activity),Util.getSdCardPath());
		}
		
	}
	
	/**
     * Set text input listener for receiving soft input.
     * @param textInputListener TextInputListener
     */
    public void setTextInputListener(TextInputListener textInputListener) {
        this.textInputListener = textInputListener;
    }
	
	/**
     * Update measured dimension of the view.
     * 
     * @param widthMeasure  the measured width of the view
     * @param heightMeasure the measured height of the view
     */
    public void updateMeasuredDimension(int measuredWidth, int measuredHeight) {
        this.setMeasuredDimension(measuredWidth, measuredHeight);
    }
     
    /**
     * Enable soft input area
     * @param enable
     */
    public void enableSoftInput(boolean enable) {
        this.useSoftInput = enable;
         
        setFocusable(enable);
        setFocusableInTouchMode(enable);
    }
 
    /**
     * Hide soft input window when IME_ACTION_DONE is performed.
     * @param hide
     */
    public void hideSoftInputWhenDone(boolean hide) {
        this.hideSoftInputWhenDone = hide;
    }
 
    /**
     * Show soft input to interact with the view.
     */
    public void showSoftInput() {
        showSoftInput(InputMethodManager.SHOW_IMPLICIT);
    }
     
    /**
     * Show soft input to interact with the view with given flags.
     * @param flags Provides additional operating flags.
     */
    public void showSoftInput(int flags) {
        InputMethodManager manager = (InputMethodManager)
            getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        manager.showSoftInput(this, flags);
    }
     
    /**
     * Hide soft input
     */
    public void hideSoftInput() {
        hideSoftInput(0);
    }
     
    /**
     * Hide soft input with given flags
     * @param flags
     */
    public void hideSoftInput(int flags) {
        InputMethodManager im = (InputMethodManager)
            getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        im.hideSoftInputFromWindow(getWindowToken(), flags);
    }
     
    /**
     * Returns true if the view is currently active in the input method.
     * @return true if the view is currently active in the input method
     */
    public boolean isSoftInputActive() {
        InputMethodManager im = (InputMethodManager)
            getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        return im.isActive(this);
    }
     
    /**
     * This method toggles the input method window display.
     * @param showFlags Provides additional operating flags
     * @param hideFlags Provides additional operating flags
     */
    public void toggleSoftInput(int showFlags, int hideFlags) {
        InputMethodManager im = (InputMethodManager)
            getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        im.toggleSoftInput(showFlags, hideFlags);
    }
 
    /**
     * Create a new InputConnection for an InputMethod to interact with the view.
     */
    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        if (!useSoftInput) return super.onCreateInputConnection(outAttrs);
         
        outAttrs.initialCapsMode = 0;
        outAttrs.initialSelEnd = outAttrs.initialSelStart = -1;
        outAttrs.inputType = (InputType.TYPE_CLASS_TEXT |
                InputType.TYPE_TEXT_VARIATION_NORMAL |
                InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE);
        outAttrs.imeOptions = (EditorInfo.IME_ACTION_DONE |
                EditorInfo.IME_FLAG_NO_EXTRACT_UI);
 
        return onLoadInputConnection();
    }
     
    /**
     * Load InputConnection to interact with the view.
     * @return InputConnection
     */
    protected InputConnection onLoadInputConnection() {
        return new TextInputConnection(this);
    }
     
    /**
     * TextInputConnection to activate soft input.
     */
	
	/**
     * TextInputConnection to activate soft input.
     */
    private class TextInputConnection extends BaseInputConnection {
        public TextInputConnection(AndroidGLView view) {
            super(view, false);
        }
 
        @Override
        public boolean performEditorAction(int actionCode) {
            if (hideSoftInputWhenDone && actionCode == EditorInfo.IME_ACTION_DONE) {
                hideSoftInput();
            }
            return super.performEditorAction(actionCode);
        }
         
        @Override
        public boolean commitText(CharSequence text, int newCursorPosition) {
            if (textInputListener != null) {
                if (textInputListener.onCommitText(text, newCursorPosition)) {
                    return true;
                }
            }
            return super.commitText(text, newCursorPosition);
        }
    }
	
	static {
    	System.loadLibrary("yon");
    }
	
}