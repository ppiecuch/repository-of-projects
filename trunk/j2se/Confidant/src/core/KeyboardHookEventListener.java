package core;

import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.KeyboardHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public abstract class KeyboardHookEventListener implements HookEventListener{
	
	public void acceptHookData(HookData arg0) {
		KeyboardHookData khd = ((KeyboardHookData) arg0);
		{
			if(khd.getTransitionState())		//处理按下事件
			{
				doPress(khd.getWParam());
			}
			else
			{
				doReleased(khd.getWParam());	//处理释放事件
			}
		}
	}
	public abstract void doPress(int keyNum);
	public abstract void doReleased(int keyNum);
}