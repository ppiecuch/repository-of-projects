package extend;

import java.awt.AWTException;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.event.InputEvent;

import org.sf.feeling.swt.win32.extension.hook.Hook;

public class ZhuXianSwifter {
	
	public static final int NUM_1=97;
	public static final int NUM_2=98;
	public static final int NUM_3=99;
	public static final int NUM_4=100;
	public static final int NUM_5=101;
	public static final int F_11=122;
	public static final int F_12=123;
	
	private static final String OFFSET_SERIES_640_480="-125,84,-125,107,-125,130,-125,152,44,0,20,0,0,0";
	private static final String OFFSET_SERIES_800_600="-156,105,-156,134,-156,163,-156,190,55,0,25,0,0,0";
	private static final String OFFSET_SERIES_1024_768="-200,138,-200,172,-200,211,-200,248,70,0,32,0,0,0";

	/**
	 * 使用说明：
	 * 1、启动后先选择所使用的分辨率，目前只支持640*480,1024*768；3
	 * 2、然后使用鼠标右键点击一下试炼答题窗口的第一个幸运星的中心点即可；
	 * 3、使用小键盘的1234选择答案，使用5点星星(第一个使用完会自动用第二个),
	 * 4、只支持命令行模式
	 * 5、F11为取坐标模式，按F11开始，再次按F11结束，并将零坐标跟之前的偏移坐标复制到系统剪贴板
	 * 6、按F12退出程序
	 * @throws AWTException 
	 */
	public static void main(String[] args) throws AWTException {
		
		/*注册鼠标Hook*/
		final MyMouseHookEventListener mouseListener=new MyMouseHookEventListener();
		Hook.MOUSE.addListener(mouseListener);
		Hook.MOUSE.install();
		
		/*系统剪贴板*/
		final Clipboard systemClipboard = Toolkit.getDefaultToolkit().getSystemClipboard(); 
		
		final Robot robot=new Robot();
		
		/*键盘监听器*/
		final KeyboardHookEventListener keyboardListener=new KeyboardHookEventListener(){
			
			private boolean haveChooseMode=false;
			private int count=0;

			@Override
			public void doPress(int keyNum) {
				String mode="";
				if(keyNum==F_12)
				{
					if(!mouseListener.getDimensionSeries().equals(""))
					{
						System.out.println("内容已经复制到系统剪贴板");
						Transferable text = new StringSelection(mouseListener.getDimensionSeries());
						systemClipboard.setContents(text,null);
					}
					System.out.println("------程序退出------");
					System.exit(0);
				}
				else if(keyNum==F_11)
				{
					haveChooseMode=false;
					count=0;
					System.out.println("启动重置模式");
					printChooseMode();
				}
				else
				{
					if(haveChooseMode==false)
					{
						switch(keyNum)
						{
						case NUM_1:
							mode="640*480";
							mouseListener.setCurrentOffsetSeries(OFFSET_SERIES_640_480);
							break;
						case NUM_2:
							mode="800*600";
							mouseListener.setCurrentOffsetSeries(OFFSET_SERIES_800_600);
							break;
						case NUM_3:
							mode="1024*768";
							mouseListener.setCurrentOffsetSeries(OFFSET_SERIES_1024_768);
							break;
						default:
							System.out.println("请重新选择:");
							printChooseMode();
							return;
						}
						System.out.println("您选择了"+mode+"分辨率模式");
						haveChooseMode=true;
						mouseListener.resetZeroDimension();
						printFetchZeroCoordinate();
					}
					else
					{
						switch (keyNum) {
						case NUM_1:
							robot.mouseMove((int)mouseListener.getDimensions().get(0).getWidth(),(int)mouseListener.getDimensions().get(0).getHeight());
							robot.mousePress(InputEvent.BUTTON1_MASK);
							robot.mouseRelease(InputEvent.BUTTON1_MASK);
							break;
						case NUM_2:
							robot.mouseMove((int)mouseListener.getDimensions().get(1).getWidth(),(int)mouseListener.getDimensions().get(1).getHeight());
							robot.mousePress(InputEvent.BUTTON1_MASK);
							robot.mouseRelease(InputEvent.BUTTON1_MASK);
							break;
						case NUM_3:
							robot.mouseMove((int)mouseListener.getDimensions().get(2).getWidth(),(int)mouseListener.getDimensions().get(2).getHeight());
							robot.mousePress(InputEvent.BUTTON1_MASK);
							robot.mouseRelease(InputEvent.BUTTON1_MASK);
							break;
						case NUM_4:
							robot.mouseMove((int)mouseListener.getDimensions().get(3).getWidth(),(int)mouseListener.getDimensions().get(3).getHeight());
							robot.mousePress(InputEvent.BUTTON1_MASK);
							robot.mouseRelease(InputEvent.BUTTON1_MASK);
							break;
						case NUM_5:
							robot.mouseMove((int)mouseListener.getDimensions().get(4+count).getWidth(),(int)mouseListener.getDimensions().get(4+count).getHeight());
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
				}
			}

			@Override
			public void doReleased(int keyNum) {}
			
		};
		
		Hook.KEYBOARD.addListener(keyboardListener);
		Hook.KEYBOARD.install(); // 註冊事件
		
		printChooseMode();

	}
	
	private static void printChooseMode()
	{
		System.out.println("请选择屏幕分辨率：");
		System.out.println("NUM1:640*480");
		System.out.println("NUM2:800*600");
		System.out.println("NUM3:1024*768");
	}
	
	private static void printFetchZeroCoordinate()
	{
		System.out.println("请在第一个幸运星的中心上点击鼠标右键");
	}

}
