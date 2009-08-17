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
	 * ʹ��˵����
	 * 1����������ѡ����ʹ�õķֱ��ʣ�Ŀǰֻ֧��640*480,1024*768��3
	 * 2��Ȼ��ʹ������Ҽ����һ���������ⴰ�ڵĵ�һ�������ǵ����ĵ㼴�ɣ�
	 * 3��ʹ��С���̵�1234ѡ��𰸣�ʹ��5������(��һ��ʹ������Զ��õڶ���),
	 * 4��ֻ֧��������ģʽ
	 * 5��F11Ϊȡ����ģʽ����F11��ʼ���ٴΰ�F11�����������������֮ǰ��ƫ�����긴�Ƶ�ϵͳ������
	 * 6����F12�˳�����
	 * @throws AWTException 
	 */
	public static void main(String[] args) throws AWTException {
		
		/*ע�����Hook*/
		final MyMouseHookEventListener mouseListener=new MyMouseHookEventListener();
		Hook.MOUSE.addListener(mouseListener);
		Hook.MOUSE.install();
		
		/*ϵͳ������*/
		final Clipboard systemClipboard = Toolkit.getDefaultToolkit().getSystemClipboard(); 
		
		final Robot robot=new Robot();
		
		/*���̼�����*/
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
						System.out.println("�����Ѿ����Ƶ�ϵͳ������");
						Transferable text = new StringSelection(mouseListener.getDimensionSeries());
						systemClipboard.setContents(text,null);
					}
					System.out.println("------�����˳�------");
					System.exit(0);
				}
				else if(keyNum==F_11)
				{
					haveChooseMode=false;
					count=0;
					System.out.println("��������ģʽ");
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
							System.out.println("������ѡ��:");
							printChooseMode();
							return;
						}
						System.out.println("��ѡ����"+mode+"�ֱ���ģʽ");
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
		Hook.KEYBOARD.install(); // �]���¼�
		
		printChooseMode();

	}
	
	private static void printChooseMode()
	{
		System.out.println("��ѡ����Ļ�ֱ��ʣ�");
		System.out.println("NUM1:640*480");
		System.out.println("NUM2:800*600");
		System.out.println("NUM3:1024*768");
	}
	
	private static void printFetchZeroCoordinate()
	{
		System.out.println("���ڵ�һ�������ǵ������ϵ������Ҽ�");
	}

}
