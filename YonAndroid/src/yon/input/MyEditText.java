package yon.input;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.widget.EditText;

/**
 * copy from qqyujian
 * @author Micheal
 *
 */
public class MyEditText extends EditText{
	
	public MyEditText(Context context) {
		super(context);
	}

	public MyEditText(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public MyEditText(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	@Override
	public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
		InputConnection conn = super.onCreateInputConnection(outAttrs);
		//将回车按键替换为DONE按键
		//TODO 不懂
		outAttrs.imeOptions &= ~EditorInfo.IME_FLAG_NO_ENTER_ACTION;
		return conn;
	}
	
	/**
	 * Dispatch a key event before it is processed by any input method associated with the view hierarchy. 
	 * This can be used to intercept key events in special situations before the IME consumes them; 
	 * a typical example would be handling the BACK key to update the application's UI instead of allowing the IME to 
	 * see it and close itself.
	 */
	@Override
	public boolean dispatchKeyEventPreIme(KeyEvent event) {
		int i = event.getKeyCode();
		Log.i("EditText","dispatchKeyEventPreIme:"+event.getKeyCode());
	    if ((KeyEvent.KEYCODE_BACK == i) && (event.getAction() == KeyEvent.ACTION_UP))
	    {
	    	//Called when an attached input method calls InputConnection.performEditorAction() for this text view
	    	
	    	//Bits of IME_MASK_ACTION: the action key performs a "done" operation, typically meaning the IME will be closed.
	    	Log.i("EditText","EditorInfo.IME_ACTION_DONE");
	    	onEditorAction(EditorInfo.IME_ACTION_DONE);
	    }
	    return super.dispatchKeyEventPreIme(event);
	}


}
