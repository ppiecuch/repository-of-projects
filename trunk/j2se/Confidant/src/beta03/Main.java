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
		/*初始化键盘事件监听器*/
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
		System.out.println("【√】初始化键盘事件监听器完成");
		/*初始化鼠标事件监听器*/
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
		System.out.println("【√】初始化鼠标事件监听器完成");
		/*初始化菜单模式*/
		initMenu();
		System.out.println("【√】初始化菜单模式完成");
		/*初始化模拟模式*/
		initImitate();
		System.out.println("【√】初始化模拟模式完成");
		
		/*启用菜单模式*/
	}
	
	private void run(){
//		/*Executive启动*/
//		executive.start();
//		System.out.println("【√】命令执行模块启动完成");
//		/*CommanderThread启动*/
//		//TODO test
//		Commander commander=commanders.get(0);
//		commander.start();
//		System.out.println("【√】循环命令模块:"+commander.getId()+"启动完成");
		
		Hook.MOUSE.addListener(mouseListener);
		Hook.KEYBOARD.addListener(keyboardListener);
		
		Hook.MOUSE.install();
		Hook.KEYBOARD.install();
		System.out.println("【√】监听器启动完成");
		
		System.out.println("====================================================");
		System.out.println("                   按键精灵 2.0版                    ");
		System.out.println("====================================================");
		System.out.println("本程序版权归夜梦星辰所有，联系方式：babala_234@163.com");
		System.out.println();
		currentMode=menuMode;
		menuMode.start();
	}
	
	private void initMenu()
	{
		menuMode=new Mode() {
			
			public void start() {
				System.out.println("请选择模式：");
				System.out.println("NUM1:记录模式(暂不支持)");
				System.out.println("F11:模拟模式");
				System.out.println("鼠标中键:获取像素信息");
				System.out.println("F12:退出系统");
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
					System.out.println("【!】无法获取Robot,屏幕像素无法获取!");
				}
			}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
	}
	
	private void exit(){
		/*解除鼠标、键盘监听器*/
		Hook.MOUSE.removeListener(mouseListener);
		Hook.KEYBOARD.removeListener(keyboardListener);
		System.out.println("【√】监听器关闭完成--退出系统");
		System.exit(0);
	}
	
	private void initImitate()
	{
		imitateMode=new Mode() {
			
			public void start() {
				/*载入XML*/
				try {
					commanders=Config.load();
					System.out.println("	【√】XML文件导入完成");
				} catch (XmlException e) {
					System.err.println("【╳】导入XML文件信息出错,请确认XML的信息及格式正确!");
					exit();
				} catch (IOException e) {
					System.err.println("【╳】XML文件读取出错,请确认XML文件是否存在且XML内容正确!");
					exit();
				}
				/*初始化Executive*/
				executive=TaskThread.getInstance("Executive");
				for(Commander c:commanders)
				{
					c.setExecutive(executive);
				}
				System.out.println("	【√】初始化命令执行模块完成");
				/*Executive启动*/
				executive.start();
				System.out.println("【√】命令执行模块启动完成");
				/*CommanderThread启动*/
				for(Commander commander:commanders)
				{
					commander.start();
					System.out.println("	【√】命令模块:"+commander.getId()+"启动完成");
				}
				
			}
			
			public void doKeyPress(int keyNum) {
				if(keyNum==Mode.F_12)
				{
					/*CommanderThread销毁*/
					for(Commander c:commanders)
						c.close();
					System.out.println("【√】循环命令模块关闭完成");
					/*Executive销毁*/
					executive.close();
					System.out.println("【√】命令执行模块关闭完成");
					currentMode=menuMode;
					menuMode.start();
					System.out.println("--退出模拟模式");
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
