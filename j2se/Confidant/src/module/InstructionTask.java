package module;

import java.awt.AWTException;
import java.awt.Point;
import java.awt.Robot;
import java.awt.event.InputEvent;

import core.Task;

/**
 * 执行指令的任务
 * @author Micheal
 *
 */
public class InstructionTask implements Task {
	
	private static Robot robot;
	
	static {
		try {
			robot=new Robot();
		} catch (AWTException e) {
			e.printStackTrace();
		}
	}
	
	private Integer instruction;
	private Point point;
	
	public InstructionTask(){}
	
	public InstructionTask(Integer instruction)
	{
		this.instruction=instruction;
		this.point=null;
	}
	
	public InstructionTask(Point point)
	{
		this.point=point;
		this.instruction=null;
	}
	
	public Integer getInstruction() {
		return instruction;
	}

	public void setInstruction(Integer instruction) {
		this.instruction = instruction;
		this.point=null;
	}

	public Point getPoint() {
		return point;
	}

	public void setPoint(Point point) {
		this.point = point;
		this.instruction=null;
	}

	public void perform() {
//		System.out.println("perform:"+toString());
		if(this.instruction!=null)
		{
			robot.keyPress(instruction);
			robot.keyRelease(instruction);
		}
		else if(this.point!=null)
		{
			robot.mouseMove(point.x,point.y);
			robot.mousePress(InputEvent.BUTTON1_MASK);
			robot.mouseRelease(InputEvent.BUTTON1_MASK);
		}
		else
		{
			System.out.println("!警告:非可执行操作!");
		}
		
	}
	
	@Override
	public String toString() {
		if(this.instruction!=null)
		{
			return String.valueOf(this.instruction);
		}
		else if(this.point!=null)
		{
			return this.point.toString();
		}
		else
		{
			return "nothing";
		}
	}

}
