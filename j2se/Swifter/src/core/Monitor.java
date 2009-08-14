package core;

import java.awt.AWTException;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.event.InputEvent;

import org.sf.feeling.swt.win32.extension.hook.Hook;

public class Monitor {
	
	public static final int NUM_0=96;
	public static final int NUM_1=97;
	public static final int NUM_2=98;
	public static final int NUM_3=99;
	public static final int NUM_4=100;
	public static final int NUM_5=101;

	/**
	 * 按下鼠标右键取坐标，总共取七次按键，其中前四次分别对应小键盘的1234，
	 * 后面三次都对应小键盘的5，小键盘0为重新取键
	 * */
	public static void main(String[] args) throws AWTException {
		
		/*注册鼠标Hook*/
		final MouseHookEventListener mouseListener=new MouseHookEventListener();
		Hook.MOUSE.addListener(mouseListener);
		Hook.MOUSE.install();
		
		final Clipboard systemClipboard = Toolkit.getDefaultToolkit().getSystemClipboard(); 
		
		final Robot robot=new Robot();
		
		KeyboardHookEventListener keyboardListener=new KeyboardHookEventListener(){
			int count=0;

			@Override
			public void doPress(int keyNum) {
				switch (keyNum) {
				case NUM_0:
					System.out.println("reset");
					mouseListener.reset();
					break;
				case NUM_1:
					robot.mouseMove((int)mouseListener.getDimentions()[0].getWidth(),(int)mouseListener.getDimentions()[0].getHeight());
					robot.mousePress(InputEvent.BUTTON1_MASK);
					robot.mouseRelease(InputEvent.BUTTON1_MASK);
					break;
				case NUM_2:
					robot.mouseMove((int)mouseListener.getDimentions()[1].getWidth(),(int)mouseListener.getDimentions()[1].getHeight());
					robot.mousePress(InputEvent.BUTTON1_MASK);
					robot.mouseRelease(InputEvent.BUTTON1_MASK);
					break;
				case NUM_3:
					robot.mouseMove((int)mouseListener.getDimentions()[2].getWidth(),(int)mouseListener.getDimentions()[2].getHeight());
					robot.mousePress(InputEvent.BUTTON1_MASK);
					robot.mouseRelease(InputEvent.BUTTON1_MASK);
					break;
				case NUM_4:
					robot.mouseMove((int)mouseListener.getDimentions()[3].getWidth(),(int)mouseListener.getDimentions()[3].getHeight());
					robot.mousePress(InputEvent.BUTTON1_MASK);
					robot.mouseRelease(InputEvent.BUTTON1_MASK);
					break;
				case NUM_5:
					robot.mouseMove((int)mouseListener.getDimentions()[4+count].getWidth(),(int)mouseListener.getDimentions()[4+count].getHeight());
					robot.mousePress(InputEvent.BUTTON1_MASK);
					robot.mouseRelease(InputEvent.BUTTON1_MASK);
					count++;
					if(count==3)
					{
						count=0;
					}
					break;
				default:
					break;
				}
				
			}

			@Override
			public void doExit() {
				mouseListener.printDimensionSeries();
				System.out.println("内容已经复制到系统剪贴板");
				Transferable text = new StringSelection(mouseListener.getDimensionSeries());
				systemClipboard.setContents(text,null);
				System.out.println("------程序退出------");
			}

			@Override
			public void doSystemIn(String cmd) {
				mouseListener.importDimensionSeries(cmd);
				System.out.println("导入坐标序列完成，你现在可以使用键盘控制了");
			}
			
		};
		
		Hook.KEYBOARD.addListener(keyboardListener);
		Hook.KEYBOARD.install(); // 註冊事件ajmnliorfc
		
		System.out.println("请在相应位置点击鼠标右键以获取第一个坐标..");

	}
	
	
	
	

}
