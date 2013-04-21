package beta04;

import java.awt.AWTException;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
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

//Ctrl+F9:�������������
//F11:�ش�
//F12:�˳�

//Ctrl+F1:���ô�һ������
//Ctrl+F2:���ô𰸶�������
//Ctrl+F3:���ô���������
//Ctrl+F4:���ô��Ľ�����
//Ctrl+F5:���ûش�һ��ѡ��
//Ctrl+F6:���ûش����ѡ��
//Ctrl+F7:���ûش�����ѡ��
//Ctrl+F8:���ûش��Ĺ�ѡ��
public class Main4 {
	
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
		
		AnswerTask.setQuestionsDir(questionsDir);
		AnswerTask.setAnswersDir(answersDir);
		AnswerTask.init();
		
		thread=TaskThread.getInstance("Main4");
		thread.start();
		
		
		/*��ʼ�������¼�������*/
		keyboardListener=new KeyboardHookEventListener(){

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
								System.out.println(keyNames.get(keyNum)+":"+rect);
							}
							
						};
						GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
						gd.setFullScreenWindow(s);
					}
					break;
				case F_11:
					if(isAltPressed())
					{
						if(questionRect==null)
							questionRect=rectMap.get(F_9);
						if(optionRects==null)
						{
							optionRects=new Rect[4];
							optionRects[0]=rectMap.get(F_1);
							optionRects[1]=rectMap.get(F_2);
							optionRects[2]=rectMap.get(F_3);
							optionRects[3]=rectMap.get(F_4);
						}
						if(answerRects==null)
						{
							answerRects=new Rect[4];
							answerRects[0]=rectMap.get(F_5);
							answerRects[1]=rectMap.get(F_6);
							answerRects[2]=rectMap.get(F_7);
							answerRects[3]=rectMap.get(F_8);
						}
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
		System.out.println("���̡���ʼ�������¼����������");
		
		Hook.KEYBOARD.addListener(keyboardListener);
		Hook.KEYBOARD.install();
		System.out.println("���̡��������������");
	}
	
	private static void exit(){
		/*���̼�����*/
		thread.close();
		Hook.KEYBOARD.removeListener(keyboardListener);
		System.out.println("���̡��������ر����--�˳�ϵͳ");
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
