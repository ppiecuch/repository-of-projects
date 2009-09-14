package module;

import java.awt.Point;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import org.sf.feeling.swt.win32.extension.hook.Hook;

import core.KeyboardHookEventListener;
import core.MouseHookEventListener;

/**
 * 记录器
 * @author Micheal
 * 
 */
public final class Recorder {
	
	private static Long latestRecordTime;
	private static List<Operation> operations=new LinkedList<Operation>();;
	private static MouseHookEventListener mouseListener;
	private static KeyboardHookEventListener keyboardListener;
	
	public static void start(){
		/*清空数据*/
		latestRecordTime=new Date().getTime();
		operations=new LinkedList<Operation>();
		
		if(mouseListener==null)
		{
			mouseListener=new MouseHookEventListener(){
				
				private Point pressPoint=new Point(-65525,-65525);
				private Point releasePoint=new Point(-65525,-65525);

				@Override
				protected void doLeftPressed(int x, int y) {
					pressPoint.setLocation(x, y);
				}

				@Override
				protected void doLeftReleased(int x, int y) {
					releasePoint.setLocation(x, y);
					
					/*
					 * 如果是click(press与release在同一个位置)则记录
					 */
					if(releasePoint.equals(pressPoint))
					{
						Long currentTime=new Date().getTime();
						Long delay=currentTime-latestRecordTime;
						
						Operation operation=new Operation();
						operation.setPoint(new Point(pressPoint.x,pressPoint.y));
						operation.setDelay(delay);
						
						latestRecordTime=currentTime;
						
						operations.add(operation);
					}
					
					//清空数据
					clearPoint(pressPoint);
					clearPoint(releasePoint);
				}

				@Override
				protected void doMiddlePressed(int x, int y) {}
				@Override
				protected void doMiddleReleased(int x, int y) {}
				@Override
				protected void doRightPressed(int x, int y) {}
				@Override
				protected void doRightReleased(int x, int y) {}
				};
			Hook.MOUSE.addListener(mouseListener);
		}
		if(keyboardListener==null)
		{
			KeyboardHookEventListener keyboardListener=new KeyboardHookEventListener(){

				@Override
				public void doPress(int keyNum) {}

				@Override
				public void doReleased(int keyNum) {
					Long currentTime=new Date().getTime();
					Long delay=currentTime-latestRecordTime;
					
					Operation operation=new Operation();
					operation.setInstruction(keyNum);
					operation.setDelay(delay);
					
					latestRecordTime=currentTime;
					
					operations.add(operation);
				}
			};
			Hook.KEYBOARD.addListener(keyboardListener);
		}
		Hook.MOUSE.install();
		Hook.KEYBOARD.install();
	}
	
	public static void stop(){
		Hook.MOUSE.uninstall();
		Hook.KEYBOARD.uninstall();
	}
	
	private static void clearPoint(Point point)
	{
		point.setLocation(-65525,-65525);
	}

	public static List<Operation> getOperations() {
		return operations;
	}

	public static void main(String[] args) throws InterruptedException
	{
		start();
		Thread.sleep(10000);
		stop();
	}

}
