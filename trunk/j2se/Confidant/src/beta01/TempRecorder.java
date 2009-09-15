package beta01;

import java.awt.Point;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import module.Operation;

public class TempRecorder {
	
	private static Long latestRecordTime;
	private static List<Operation> operations=new LinkedList<Operation>();
	private static Point pressPoint=new Point(-65525,-65525);
	private static Point releasePoint=new Point(-65525,-65525);
	
	public static void start(){
		/*�������*/
		latestRecordTime=new Date().getTime();
		operations=new LinkedList<Operation>();
	}
	
	public static void doPress(int keyNum){	
		Long currentTime=new Date().getTime();
		Long delay=currentTime-latestRecordTime;
		
		Operation operation=new Operation();
		operation.setInstruction(keyNum);
		operation.setDelay(delay);
		
		latestRecordTime=currentTime;
		operations.add(operation);
		
		System.out.println("��¼������"+operation);
	}
	
	public static void doReleased(int keyNum) {}
	
	public static void doLeftPressed(int x, int y) {
		pressPoint.setLocation(x, y);
	}

	public static void doLeftReleased(int x, int y) {
		releasePoint.setLocation(x, y);
		
		/*
		 * �����click(press��release��ͬһ��λ��)���¼
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
			
			System.out.println("��¼������"+operation);
		}
		
		//�������
		clearPoint(pressPoint);
		clearPoint(releasePoint);
	}
	
	private static void clearPoint(Point point)
	{
		point.setLocation(-65525,-65525);
	}

	public static List<Operation> getOperations() {
		return operations;
	}

}
