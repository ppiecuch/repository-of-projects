package core;

import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.MouseHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public abstract class MouseHookEventListener implements HookEventListener{
	
	public void acceptHookData(HookData hookData) {
		int x=((MouseHookData) hookData).getPointX();
		int y=((MouseHookData) hookData).getPointY();
		switch(hookData.getWParam())
		{
		case 513:
			doLeftPressed(x,y);
			break;
		case 514:
			doLeftReleased(x,y);
			break;
		case 516:
			doRightPressed(x,y);
			break;
		case 517:
			doRightReleased(x,y);
			break;
		case 519:
			doMiddlePressed(x,y);
			break;
		case 520:
			doMiddleReleased(x,y);
			break;
		default:
		}
	}

	protected abstract void doLeftPressed(int x,int y);
	protected abstract void doLeftReleased(int x,int y);
	protected abstract void doRightPressed(int x,int y);
	protected abstract void doRightReleased(int x,int y);
	protected abstract void doMiddlePressed(int x,int y);
	protected abstract void doMiddleReleased(int x,int y);
}