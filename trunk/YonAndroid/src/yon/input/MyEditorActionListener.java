package yon.input;

import yon.demo.Constant;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class MyEditorActionListener implements OnEditorActionListener {
	
	Handler handler;
	MyEditText editText;
	
	public MyEditorActionListener(MyEditText editText,Handler handler) {
		this.handler=handler;
		this.editText=editText;
	}

	/**
	 * Called when an action is being performed.
	 */
	@Override
	public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
		Log.i("EditText","onEditorAction:"+actionId+","+event);
		if(actionId==EditorInfo.IME_ACTION_NEXT||actionId==EditorInfo.IME_NULL||actionId==EditorInfo.IME_ACTION_DONE)
		{
			if(event==null||event.getAction()==KeyEvent.ACTION_UP)
			{
				String text = editText.getText().toString();
				Message msg=handler.obtainMessage(Constant.MSG_COMPLETE_INPUT);
				Bundle bundle=new Bundle();
				bundle.putString(Constant.MSG_KEY_CONTENT, text);
				msg.setData(bundle);
				handler.sendMessage(msg);
				Log.i("EditText","MSG_HIDE");
			}
		}
		return false;
	}

}
