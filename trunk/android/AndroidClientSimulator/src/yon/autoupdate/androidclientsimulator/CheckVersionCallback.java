package yon.autoupdate.androidclientsimulator;

import android.os.Handler;
import android.os.Message;
import yon.autoupdate.net.Callback;

public class CheckVersionCallback implements Callback,Constant {
	
	private Handler handler;
	
	public CheckVersionCallback(Handler handler){
		this.handler=handler;
	}

	@Override
	public void doTimeout() {
		Message msg=handler.obtainMessage(MSG_NETWORK_TIMEOUT);
		handler.sendMessage(msg);
	}

	@Override
	public void doFailed() {
		Message msg=handler.obtainMessage(MSG_NETWORK_FAILED);
		handler.sendMessage(msg);
	}

	@Override
	public void doCompleted() {

	}

}
