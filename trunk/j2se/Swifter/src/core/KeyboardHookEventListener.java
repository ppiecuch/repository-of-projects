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
			if(khd.getTransitionState())		//�������¼�
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
		if (khd.getScanCode()==F_12&&khd.getTransitionState()) { // ����[F12]�Y��
			doExit();
			System.out.println("------�����˳�------");
			System.exit(0);
		}
		if (khd.getScanCode()==F_11&&khd.getTransitionState()) { // ����[F11]�Y��
			System.out.println("������ƫ���������У��ö��Ÿ���������ʹ��ϵͳ���ɵ����У���");
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