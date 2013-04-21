package beta03;

import java.awt.AWTException;
import java.awt.Robot;

public class RobotCommand extends Command {
	
	private static Robot robot;
	
	protected Robot getRobot() throws AWTException
	{
		if(RobotCommand.robot==null)
			RobotCommand.robot=new Robot();
		return RobotCommand.robot;
	}

	public void perform() {
		// TODO Auto-generated method stub

	}

}
