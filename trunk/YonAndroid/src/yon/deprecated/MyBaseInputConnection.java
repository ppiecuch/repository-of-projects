package yon.deprecated;

import android.util.Log;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;

/**
 * BaseInputConnection相当于一个InputMethodService和View之间的一个通道。
 * 每当InputMethodService产生一个结果时，都会调用BaseInputConnection的commitText方法，把结果传递出来。 
 * copy from http://king523103.iteye.com/blog/1190100
 * @author Micheal
 *
 */
public class MyBaseInputConnection extends BaseInputConnection {
	
	public String result="";

	public MyBaseInputConnection(View targetView, boolean fullEditor) {
		super(targetView, fullEditor);
	}
	
	//输入法程序就是通过调用这个方法把最终结果输出来的。
	@Override
	public boolean commitText(CharSequence text, int newCursorPosition) {
		result=text.toString();
		Log.i("Connection",result);
		return true;
	}

}
