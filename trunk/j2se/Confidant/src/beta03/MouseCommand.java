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
			System.out.println("【debug】按Mouse:"+getMouse());
		} catch (AWTException e) {
			System.out.println("【!】无法获取Robot,部分鼠标模拟指令无法执行!");
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
			return "左:"+x+","+y;
		if(type==TYPE_RIGHT_MOUSE)
			return "右:"+x+","+y;
		return null;
	}
}
