package beta03;

import java.awt.AWTException;
import java.awt.event.InputEvent;

public class MouseCommand extends RobotCommand {
	
	private int x;
	private int y;

	public void perform() {
		try {
			switch(type)
			{
			case TYPE_LEFT_MOUSE:
				getRobot().mouseMove(x, y);
				getRobot().mousePress(InputEvent.BUTTON1_MASK);
				getRobot().mouseRelease(InputEvent.BUTTON1_MASK);
				break;
			case TYPE_RIGHT_MOUSE:
				getRobot().mouseMove(x, y);
				getRobot().mousePress(InputEvent.BUTTON3_MASK);
				getRobot().mouseRelease(InputEvent.BUTTON3_MASK);
				break;
			}
			System.out.println("��debug����Mouse:"+getMouse());
		} catch (AWTException e) {
			System.out.println("��!���޷���ȡRobot,�������ģ��ָ���޷�ִ��!");
		}
	}

	public int getX() {
		return x;
	}

	public void setX(int x) {
		this.x = x;
	}

	public int getY() {
		return y;
	}

	public void setY(int y) {
		this.y = y;
	}

	private String getMouse()
	{
		if(type==TYPE_LEFT_MOUSE)
			return "��:"+x+","+y;
		if(type==TYPE_RIGHT_MOUSE)
			return "��:"+x+","+y;
		return null;
	}
}
