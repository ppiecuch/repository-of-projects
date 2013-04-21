package beta04;

import java.awt.AWTException;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.event.InputEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;
import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.filechooser.FileSystemView;

import org.sf.feeling.swt.win32.extension.hook.Hook;

import core.KeyboardHookEventListener;
import core.TaskThread;
import core.Tray;

//Ctrl+Alt+F9:设置选项截屏框大小
//Ctrl+Alt+F9:设置选项截屏框位置
//1:勾选A
//2:勾选B
//3:勾选C
//4:勾选D
//F11:回答
//Alt+F12:退出

//Ctrl+Alt+F1:设置答案一截屏框
//Ctrl+Alt+F2:设置答案二截屏框
//Ctrl+Alt+F3:设置答案三截屏框
//Ctrl+Alt+F4:设置答案四截屏框
//Ctrl+Alt+F5:设置回答一勾选框
//Ctrl+Alt+F6:设置回答二勾选框
//Ctrl+Alt+F7:设置回答三勾选框
//Ctrl+Alt+F8:设置回答四勾选框
public class Main4 {
	
	public static final int SHIFT=17;
	public static final int CTRL=17;
	public static final int ALT=18;
	public static final int F_1=112;
	public static final int F_2=113;
	public static final int F_3=114;
	public static final int F_4=115;
	public static final int F_5=116;
	public static final int F_6=117;
	public static final int F_7=118;
	public static final int F_8=119;
	public static final int F_9=120;
	public static final int F_10=121;
	public static final int F_11=122;
	public static final int F_12=123;
	
	public static final int KEY_1=49;
	public static final int KEY_2=50;
	public static final int KEY_3=51;
	public static final int KEY_4=52;
	
	public static final int KEY_A=65;
	public static final int KEY_B=66;
	public static final int KEY_C=67;
	public static final int KEY_D=68;
	
	private static KeyboardHookEventListener keyboardListener=null;
	private static TaskThread thread=null;
	private static boolean ctrlPressed=false;
	private static boolean altPressed=false;
	
	private static Rect questionRect=null;
	private static Rect[] optionRects=null;
	private static Rect[] answerRects=null;
	
	public static HashMap<Integer, String> keyNames=new HashMap<Integer, String>();
	public static HashMap<Integer, Rect> rectMap=new HashMap<Integer, Rect>();
	
	public static boolean isCtrlPressed(){
		return ctrlPressed;
	}
	
	public static boolean isAltPressed(){
		return altPressed;
	}
	
	public static void answer(int option) throws AWTException{
		Rect r=null;
		switch(option)
		{
		case KEY_1:
			r=rectMap.get(F_5);
			break;
		case KEY_2:
			r=rectMap.get(F_6);
			break;
		case KEY_3:
			r=rectMap.get(F_7);
			break;
		case KEY_4:
			r=rectMap.get(F_8);
			break;
		}
		if(r!=null)
		{
			Robot robot = new Robot();
			Point p=r.getCenter();
			robot.mouseMove(p.x, p.y);
			robot.mousePress(InputEvent.BUTTON1_MASK);
			try {
				Thread.sleep(2);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			robot.mouseRelease(InputEvent.BUTTON1_MASK);
		}
	}
	
	private static void init(String questionsDir,String answersDir){
		keyNames.put(F_1, "F1");
		keyNames.put(F_2, "F2");
		keyNames.put(F_3, "F3");
		keyNames.put(F_4, "F4");
		keyNames.put(F_5, "F5");
		keyNames.put(F_6, "F6");
		keyNames.put(F_7, "F7");
		keyNames.put(F_8, "F8");
		keyNames.put(F_9, "F9");
		keyNames.put(F_10, "F10");
		keyNames.put(F_11, "F11");
		keyNames.put(F_12, "F12");
		
		rectMap.put(F_1, new Rect(515,393,615,405));
		rectMap.put(F_2, new Rect(515,418,615,430));
		rectMap.put(F_3, new Rect(515,441,615,453));
		rectMap.put(F_4, new Rect(515,465,615,477));
		rectMap.put(F_5, new Rect(475,393,495,405));
		rectMap.put(F_6, new Rect(475,418,495,430));
		rectMap.put(F_7, new Rect(475,441,495,453));
		rectMap.put(F_8, new Rect(475,465,495,477));
		rectMap.put(F_9, new Rect(493,292,732,305));
		
		//test case http://www.3454.com/34453p
		/*rectMap.put(F_1, new Rect(476,460,519,475));
		rectMap.put(F_2, new Rect(476,481,519,496));
		rectMap.put(F_3, new Rect(476,502,519,517));
		rectMap.put(F_4, new Rect(476,481,519,496));
		rectMap.put(F_5, new Rect(453,459,547,476));
		rectMap.put(F_6, new Rect(456,479,537,497));
		rectMap.put(F_7, new Rect(457,501,551,518));
		rectMap.put(F_8, new Rect(453,482,536,497));
		rectMap.put(F_9, new Rect(482,349,615,375));*/
		
		AnswerTask.setQuestionsDir(questionsDir);
		AnswerTask.setAnswersDir(answersDir);
		AnswerTask.init();
		
		thread=TaskThread.getInstance("Main4");
		thread.start();
		
		
		/*初始化键盘事件监听器*/
		keyboardListener=new KeyboardHookEventListener(){
			
			private Rect optionRect=null;

			@Override
			public void doPress(int keyNum) {
				switch(keyNum)
				{
				case CTRL:
					ctrlPressed=true;
					break;
				case ALT:
					altPressed=true;
					break;
				case KEY_1:
				case KEY_2:
				case KEY_3:
				case KEY_4:
					//if(isAltPressed())
					{
						try {
							answer(keyNum);
						} catch (AWTException e) {
							e.printStackTrace();
						}
					}
					break;
				}
				//System.out.println("press:"+keyNum);
			}

			@Override
			public void doReleased(final int keyNum) {
				switch(keyNum)
				{
				case CTRL:
					ctrlPressed=false;
					break;
				case ALT:
					altPressed=false;
					break;
				case F_1:
				case F_2:
				case F_3:
				case F_4:
					if(isCtrlPressed()&&isAltPressed())
					{
						if(optionRect==null)
						{
							Tray.getInstance().tip("请先设置框选区域!");
						}
						else
						{
							Selector s=new Selector(optionRect){
								
								private static final long serialVersionUID = 1L;

								@Override
								public void doSelected(Rect rect) {
									rectMap.put(keyNum, rect);
									optionRect=rect;
									String msg="设置按键("+keyNames.get(keyNum)+")区域:"+rect;
									System.out.println(msg);
									Tray.getInstance().tip(msg);
								}
								
							};
							GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
							gd.setFullScreenWindow(s);
						}
					}
					break;
				case F_5:
				case F_6:
				case F_7:
				case F_8:
				case F_9:
					if(isCtrlPressed()&&isAltPressed())
					{
						Selector s=new Selector(){

							@Override
							public void doSelected(Rect rect) {
								rectMap.put(keyNum, rect);
								String msg="设置按键("+keyNames.get(keyNum)+")区域:"+rect;
								System.out.println(msg);
								Tray.getInstance().tip(msg);
							}
							
						};
						GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
						gd.setFullScreenWindow(s);
					}
					break;
				case F_10:
					if(isCtrlPressed()&&isAltPressed())
					{
						Selector s=new Selector(){

							private static final long serialVersionUID = 1L;

							@Override
							public void doSelected(Rect rect) {
								optionRect=rect;
								String msg="设置框选区域大小:{"+rect.getWidth()+","+rect.getHeight()+"}";
								System.out.println(msg);
								Tray.getInstance().tip(msg);
							}
							
						};
						GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
						gd.setFullScreenWindow(s);
					}
					break;
				case F_11:
					//if(isAltPressed())
					{
						questionRect=rectMap.get(F_9);
						optionRects=new Rect[4];
						optionRects[0]=rectMap.get(F_1);
						optionRects[1]=rectMap.get(F_2);
						optionRects[2]=rectMap.get(F_3);
						optionRects[3]=rectMap.get(F_4);
						answerRects=new Rect[4];
						answerRects[0]=rectMap.get(F_5);
						answerRects[1]=rectMap.get(F_6);
						answerRects[2]=rectMap.get(F_7);
						answerRects[3]=rectMap.get(F_8);
						AnswerTask task=new AnswerTask(questionRect, optionRects, answerRects);
						thread.addTask(task);
					}
					break;
				case F_12:
					if(isAltPressed())
						exit();
				}
				//System.out.println("release:"+keyNum);
			}
		};
		System.out.println("【√】初始化键盘事件监听器完成");
		
		Hook.KEYBOARD.addListener(keyboardListener);
		Hook.KEYBOARD.install();
		System.out.println("【√】监听器启动完成");
	}
	
	private static void exit(){
		/*键盘监听器*/
		thread.close();
		Hook.KEYBOARD.removeListener(keyboardListener);
		System.out.println("【√】监听器关闭完成--退出系统");
		System.exit(0);
	}

	/**
	 * @param args
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws InterruptedException {
		// TODO Auto-generated method stub
		init(args[0],args[1]);
	}

}
