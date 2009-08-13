package demo;

import org.sf.feeling.swt.win32.extension.hook.Hook;
import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.MouseHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public class HotMouse {

	public static void main(String[] args) {
		Hook.MOUSE.addListener(new HookEventListener() {
			public void acceptHookData(HookData hookData) {
				if (hookData != null) {
					// System.out.println("Mouse Point X = " +
					// ((MouseHookData)hookData).getPointX());
					// System.err.println("Mouse Point Y =" +
					// ((MouseHookData)hookData).getPointY());
					System.err.println(((MouseHookData) hookData).getPointX()
							+ "," + ((MouseHookData) hookData).getPointY());
				}
				if (hookData.getWParam() == 513) {
					System.out.println("left button pressed");// left button
																// pressed
				} else if (hookData.getWParam() == 514) {
					System.out.println("left button released");// left button
																// released
				} else if (hookData.getWParam() == 516) {
					System.out.println("right button pressed");// right button
																// pressed
				} else if (hookData.getWParam() == 517) {
					System.out.println("right button");// right button
				} else if (hookData.getWParam() == 519) {
					System.out.println("middle button 519");// middle button
				} else if (hookData.getWParam() == 520) {
					System.out.println("middle button 520");// middle button
				}
			}
		});
		Hook.MOUSE.install();
	}

}