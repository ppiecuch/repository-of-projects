package beta02;

import module.Calculagraph;

import org.sf.feeling.swt.win32.extension.hook.Hook;

import beta01.Mode;
import beta01.TempRecorder;

import core.KeyboardHookEventListener;
import core.MouseHookEventListener;

public class Main {
	
	private static Mode menuMode;
	private static Mode recordMode;
	private static Mode imitateMode;
	
	private static Mode currentMode;
	private static Calculagraph calculagraph;
	
	private static MouseHookEventListener mouseListener=null;
	private static KeyboardHookEventListener keyboardListener=null;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		keyboardListener=new KeyboardHookEventListener(){

			@Override
			public void doPress(int keyNum) {
				if(currentMode!=null)
				{
					currentMode.doPress(keyNum);
				}
			}

			@Override
			public void doReleased(int keyNum) {
				if(currentMode!=null)
				{
					currentMode.doReleased(keyNum);
				}
			}
		};
		
		mouseListener=new MouseHookEventListener(){

			@Override
			protected void doLeftPressed(int x, int y) {
				if(currentMode!=null)
				{
					currentMode.doLeftPressed(x, y);
				}
				
			}

			@Override
			protected void doLeftReleased(int x, int y) {
				if(currentMode!=null)
				{
					currentMode.doLeftReleased(x, y);
				}
			}

			@Override
			protected void doMiddlePressed(int x, int y) {
				if(currentMode!=null)
				{
					currentMode.doMiddlePressed(x, y);
				}
			}

			@Override
			protected void doMiddleReleased(int x, int y) {
				if(currentMode!=null)
				{
					currentMode.doMiddleReleased(x, y);
				}
			}

			@Override
			protected void doRightPressed(int x, int y) {
				if(currentMode!=null)
				{
					currentMode.doRightPressed(x, y);
				}
			}

			@Override
			protected void doRightReleased(int x, int y) {
				if(currentMode!=null)
				{
					currentMode.doRightReleased(x, y);
				}
			}
			
		};
		
		menuMode=new Mode(){

			public void start() {
				System.out.println("��ѡ��ģʽ��");
				System.out.println("NUM1:��¼ģʽ");
				System.out.println("NUM2:ģ��ģʽ");
				System.out.println("F12:�˳�ϵͳ");
			}

			public void doPress(int keyNum) {
				switch(keyNum)
				{
				case Mode.NUM_1:
					currentMode=recordMode;
					recordMode.start();
					break;
				case Mode.NUM_2:
					currentMode=imitateMode;
					imitateMode.start();
					break;
				case Mode.F_12:
					System.out.println("--�˳�ϵͳ--");
					System.exit(0);
					break;
				default:
				}
			}

			public void doReleased(int keyNum) {}
			public void doLeftPressed(int x, int y) {}
			public void doLeftReleased(int x, int y) {}
			public void doMiddlePressed(int x, int y) {}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
			
		};
		
		recordMode=new Mode(){

			public void doPress(int keyNum) {
				if(keyNum==Mode.F_12)
				{
					
					calculagraph=new Calculagraph("Imitator");
					calculagraph.setOperations(TempRecorder.getOperations());
					Config.saveConfig(calculagraph);
					calculagraph=null;
					System.out.println("--��¼ģʽ�ѹر�--");
					
					currentMode=menuMode;
					menuMode.start();
					return;
				}
				TempRecorder.doPress(keyNum);
			}

			public void doReleased(int keyNum) {
				TempRecorder.doReleased(keyNum);
			}

			public void start() {
				TempRecorder.start();
				System.out.println("��¼ģʽ������,ϵͳ���ڼ�¼���Ĳ���...");
			}
			
			public void doLeftPressed(int x, int y) {
				TempRecorder.doLeftPressed(x, y);
			}
			public void doLeftReleased(int x, int y) {
				TempRecorder.doLeftReleased(x, y);
			}
			public void doMiddlePressed(int x, int y) {}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
		
		imitateMode=new Mode(){

			public void doPress(int keyNum) {
				if(keyNum==Mode.F_12)
				{
					calculagraph.close();
					calculagraph=null;
					System.out.println("--ģ��ģʽ�ѹر�--");
					currentMode=menuMode;
					menuMode.start();
				}
			}

			public void doReleased(int keyNum) {}

			public void start() {
				if(calculagraph==null)
				{
					calculagraph=new Calculagraph("Imitator");
				}
				
				Config.loadConfig(calculagraph);
				calculagraph.start();
				System.out.println("ģ��ģʽ������,ϵͳ����ģ�����Ĳ���...");
			}
			
			public void doLeftPressed(int x, int y) {}
			public void doLeftReleased(int x, int y) {}
			public void doMiddlePressed(int x, int y) {}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
		
		Hook.MOUSE.addListener(mouseListener);
		Hook.KEYBOARD.addListener(keyboardListener);
		
		Hook.MOUSE.install();
		Hook.KEYBOARD.install();
		
		currentMode=menuMode;
		System.out.println("====================================================");
		System.out.println("                   �������� 1.1��                    ");
		System.out.println("====================================================");
		System.out.println("�������Ȩ��ҹ���ǳ����У���ϵ��ʽ��babala_234@163.com");
		System.out.println();
		menuMode.start();
	}

}
