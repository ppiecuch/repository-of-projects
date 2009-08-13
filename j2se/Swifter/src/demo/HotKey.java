package demo;

import org.sf.feeling.swt.win32.extension.hook.Hook;
import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.KeyboardHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public class HotKey {
	public static void main(final String[] args) {
		Hook.KEYBOARD.addListener(new HookEventListener() {// @Override
					public void acceptHookData(HookData arg0) {
						KeyboardHookData KHD = ((KeyboardHookData) arg0);
						System.out.println(KHD.getWParam() + " - "
								+ KHD.getTransitionState()); // Print code
						if (KHD.getScanCode() == 29 && KHD.getTransitionState()) { // 按下Ctrl執行動作
							System.out.println("Hotting");
						} else if (KHD.getScanCode() == 1) { // 按下[ESC]結束
							System.exit(0);
						}
					}
				});
		Hook.KEYBOARD.install(); // 註冊事件
	}
}