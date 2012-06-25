package yon.demo;

import android.os.Handler;
import android.os.Message;

public abstract class YonHandler extends Handler {
	
	
	
	@Override
	public void handleMessage(Message msg) {
		switch(msg.what){
		case Constant.MSG_SHOW_SPINNER:
			showWating(msg.getData().getString(Constant.MSG_KEY_CONTENT));
			break;
		case Constant.MSG_HIDE_SPINNER:
			hideWating();
			break;
		case Constant.MSG_SETUP_INPUT:
			setupInput();
			break;
		case Constant.MSG_TOAST:
			showToast(msg.getData().getString(Constant.MSG_KEY_CONTENT));
			break;
		case Constant.MSG_SETUP_CONFIRM:
		{
			String title=msg.getData().getString(Constant.MSG_KEY_TITLE);
			String content=msg.getData().getString(Constant.MSG_KEY_CONTENT);
			String ok=msg.getData().getString(Constant.MSG_KEY_POSITIVE_BUTTON);
			String cancel=msg.getData().getString(Constant.MSG_KEY_NEGATIVE_BUTTON);
			showConfirm(title,content,ok,cancel);
		}
			break;
		}
	}
	
	public abstract void showWating(String text);
	public abstract void hideWating();
	public abstract void setupInput();
	public abstract void showToast(String text);
	public abstract void showConfirm(String title,String content,String ok,String cancel);

}
