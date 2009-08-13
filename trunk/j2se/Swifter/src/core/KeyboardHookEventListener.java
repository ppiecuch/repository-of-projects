package core;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.KeyboardHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public abstract class KeyboardHookEventListener implements HookEventListener{
	
	public static final int F_12=88;
	public static final int F_11=87;
	
//	private boolean isF12Pressed=false;
	
	public void acceptHookData(HookData arg0) {
		KeyboardHookData khd = ((KeyboardHookData) arg0);
		{
			if(khd.getTransitionState())		//处理按下事件
			{
				doPress(khd.getWParam());
			}
		}
//		if (khd.getScanCode() == 29 && khd.getTransitionState()) {
//			System.out.println("Ctrl");
//			isF12Pressed=true;
//		}
//		else if(khd.getScanCode() == 29 && khd.getTransitionState()==false)
//		{
//			System.out.println("No Ctrl");
//			isF12Pressed=false;
//		}
//		System.out.println(khd.getScanCode());
		if (khd.getScanCode()==F_12&&khd.getTransitionState()) { // 按下[F12]結束
			doExit();
			System.out.println("------程序退出------");
			System.exit(0);
		}
		if (khd.getScanCode()==F_11&&khd.getTransitionState()) { // 按下[F11]結束
			System.out.println("请输入偏移数字序列，用逗号隔开（建议使用系统生成的序列）：");
			BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
			try {
				String cmd = reader.readLine();
				doSystemIn(cmd);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	public abstract void doSystemIn(String cmd);
	public abstract void doPress(int keyNum);
	public abstract void doExit();
}