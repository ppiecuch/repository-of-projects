package org.tinder.studio.warbird;

import android.view.KeyEvent;
import android.view.View;



public class KeyCache {

	/** 向上 */
	public static final int KEY_UP = 1;

	/** 向下 */
	public static final int KEY_DOWN = 1 << 1;

	/** 向右 */
	public static final int KEY_RIGHT = 1 << 2;

	/** 向左 */
	public static final int KEY_LEFT = 1 << 3;

	/** 5键 */
	public static final int KEY_FIRE = 1 << 4;

	/** 左软键 */
	public static final int KEY_LEFT_SOFT = 1 << 5;

	/** 右软键 */
	public static final int KEY_RIGHT_SOFT = 1 << 6;

	/** 特殊用途按键，例如0键 */
	public static final int KEY_ZERO = 1 << 7;
	
	/**1键*/
	public static final int KEY_ONE=1<<8;
	
	/**3键*/
	public static final int KEY_THREE=1<<9;
	
	/**7键*/
	public static final int KEY_SEVEN=1<<10;
	
	/**9键*/
	public static final int KEY_NINE=1<<11;

	private int keyState = 0;
	private static KeyCache instance;
	
	private KeyCache(){}

	/**
	 * 将物理键值转换为自定义键值
	 * 说明：该方法和机型相关，下面是WTK的实现
	 * @param keyCode 物理键值
	 * @return 自定义键值
	 */
	private int convertKey(int keyCode) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_DPAD_DOWN:
			return KEY_DOWN;
		case KeyEvent.KEYCODE_DPAD_UP:
			return KEY_UP;
		case KeyEvent.KEYCODE_DPAD_LEFT:
			return KEY_LEFT;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			return KEY_RIGHT;
		case KeyEvent.KEYCODE_DPAD_CENTER:
			return KEY_FIRE;
		}
		return 0;
	}

	public void keyPressed(int keyCode) {
		int key = convertKey(keyCode);
		keyState |= key;
	}

	public void keyReleased(int keyCode) {
		int key = convertKey(keyCode);
		keyState &= ~key;
	}
	
	public void clearKeyState() {
		keyState = 0;
	}

	public int getKeyState() {
		return keyState;
	}

	public static KeyCache getInstance() {
		if(instance==null)
			instance=new KeyCache();
		return instance;
	}
	
	
	
	

}
