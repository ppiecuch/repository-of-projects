package beta03;

import java.awt.AWTException;
import java.io.IOException;
import java.util.List;

import org.apache.xmlbeans.XmlException;
import org.sf.feeling.swt.win32.extension.hook.Hook;

import core.KeyboardHookEventListener;
import core.MouseHookEventListener;
import core.Snapshot;
import core.TaskThread;

public class Main {
	
	private Mode menuMode;
	private Mode imitateMode;
	private static Mode currentMode;
	private static MouseHookEventListener mouseListener=null;
	private static KeyboardHookEventListener keyboardListener=null;
	private TaskThread executive;
	private List<Commander> commanders;

	/**
	 * @param args
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws InterruptedException {
		Main main=new Main();
		main.init();
		Thread.sleep(1000);
		main.run();

	}
	
	private void init(){
		/*��ʼ�������¼�������*/
		keyboardListener=new KeyboardHookEventListener(){

			@Override
			public void doPress(int keyNum) {
				if(currentMode!=null)
				{
					currentMode.doKeyPress(keyNum);
				}
			}

			@Override
			public void doReleased(int keyNum) {
				if(currentMode!=null)
				{
					currentMode.doKeyReleased(keyNum);
				}
			}
		};
		System.out.println("���̡���ʼ�������¼����������");
		/*��ʼ������¼�������*/
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
		System.out.println("���̡���ʼ������¼����������");
		/*��ʼ���˵�ģʽ*/
		initMenu();
		System.out.println("���̡���ʼ���˵�ģʽ���");
		/*��ʼ��ģ��ģʽ*/
		initImitate();
		System.out.println("���̡���ʼ��ģ��ģʽ���");
		
		/*���ò˵�ģʽ*/
	}
	
	private void run(){
//		/*Executive����*/
//		executive.start();
//		System.out.println("���̡�����ִ��ģ���������");
//		/*CommanderThread����*/
//		//TODO test
//		Commander commander=commanders.get(0);
//		commander.start();
//		System.out.println("���̡�ѭ������ģ��:"+commander.getId()+"�������");
		
		Hook.MOUSE.addListener(mouseListener);
		Hook.KEYBOARD.addListener(keyboardListener);
		
		Hook.MOUSE.install();
		Hook.KEYBOARD.install();
		System.out.println("���̡��������������");
		
		System.out.println("====================================================");
		System.out.println("                   �������� 2.0��                    ");
		System.out.println("====================================================");
		System.out.println("�������Ȩ��ҹ���ǳ����У���ϵ��ʽ��babala_234@163.com");
		System.out.println();
		currentMode=menuMode;
		menuMode.start();
	}
	
	private void initMenu()
	{
		menuMode=new Mode() {
			
			public void start() {
				System.out.println("��ѡ��ģʽ��");
				System.out.println("NUM1:��¼ģʽ(�ݲ�֧��)");
				System.out.println("F11:ģ��ģʽ");
				System.out.println("����м�:��ȡ������Ϣ");
				System.out.println("F12:�˳�ϵͳ");
			}
			
			public void doKeyPress(int keyNum) {
				switch(keyNum)
				{
				case Mode.NUM_1:
//					currentMode=recordMode;
//					recordMode.start();
					break;
				case Mode.F_11:
					currentMode=imitateMode;
					imitateMode.start();
					break;
				case Mode.F_12:
					exit();
					break;
				default:
				}
			}

			public void doKeyReleased(int keyNum) {}
			public void doLeftPressed(int x, int y) {}
			public void doLeftReleased(int x, int y) {}
			public void doMiddlePressed(int x, int y) {
				try {
					System.out.println(x+","+y+":0x"+Integer.toHexString(Snapshot.getScreenPixelColor(x, y)).toUpperCase());
				} catch (AWTException e) {
					System.out.println("��!���޷���ȡRobot,��Ļ�����޷���ȡ!");
				}
			}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
	}
	
	private void exit(){
		/*�����ꡢ���̼�����*/
		Hook.MOUSE.removeListener(mouseListener);
		Hook.KEYBOARD.removeListener(keyboardListener);
		System.out.println("���̡��������ر����--�˳�ϵͳ");
		System.exit(0);
	}
	
	private void initImitate()
	{
		imitateMode=new Mode() {
			
			public void start() {
				/*����XML*/
				try {
					commanders=Config.load();
					System.out.println("	���̡�XML�ļ��������");
				} catch (XmlException e) {
					System.err.println("���w������XML�ļ���Ϣ����,��ȷ��XML����Ϣ����ʽ��ȷ!");
					exit();
				} catch (IOException e) {
					System.err.println("���w��XML�ļ���ȡ����,��ȷ��XML�ļ��Ƿ������XML������ȷ!");
					exit();
				}
				/*��ʼ��Executive*/
				executive=TaskThread.getInstance("Executive");
				for(Commander c:commanders)
				{
					c.setExecutive(executive);
				}
				System.out.println("	���̡���ʼ������ִ��ģ�����");
				/*Executive����*/
				executive.start();
				System.out.println("���̡�����ִ��ģ���������");
				/*CommanderThread����*/
				for(Commander commander:commanders)
				{
					commander.start();
					System.out.println("	���̡�����ģ��:"+commander.getId()+"�������");
				}
				
			}
			
			public void doKeyPress(int keyNum) {
				if(keyNum==Mode.F_12)
				{
					/*CommanderThread����*/
					for(Commander c:commanders)
						c.close();
					System.out.println("���̡�ѭ������ģ��ر����");
					/*Executive����*/
					executive.close();
					System.out.println("���̡�����ִ��ģ��ر����");
					currentMode=menuMode;
					menuMode.start();
					System.out.println("--�˳�ģ��ģʽ");
				}
			}
			public void doKeyReleased(int keyNum) {}
			public void doLeftPressed(int x, int y) {}
			public void doLeftReleased(int x, int y) {}
			public void doMiddlePressed(int x, int y) {}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
	}

}
