package beta03;

import java.awt.AWTException;
import java.awt.event.KeyEvent;

public class KeyCommand extends RobotCommand {
	
	private int value;
	private boolean withCtrl;
	private boolean withAlt;
	private boolean withShift;
	
	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}

	public boolean isWithCtrl() {
		return withCtrl;
	}

	public void setWithCtrl(boolean withCtrl) {
		this.withCtrl = withCtrl;
	}

	public boolean isWithAlt() {
		return withAlt;
	}

	public void setWithAlt(boolean withAlt) {
		this.withAlt = withAlt;
	}

	public boolean isWithShift() {
		return withShift;
	}

	public void setWithShift(boolean withShift) {
		this.withShift = withShift;
	}

	public void perform() {
		try {
			if(withAlt)
				getRobot().keyPress(KeyEvent.VK_ALT);
			if(withCtrl)
				getRobot().keyPress(KeyEvent.VK_CONTROL);
			if(withShift)
				getRobot().keyPress(KeyEvent.VK_SHIFT);
			getRobot().keyPress(value);
			getRobot().keyRelease(value);
			if(withAlt)
				getRobot().keyRelease(KeyEvent.VK_ALT);
			if(withCtrl)
				getRobot().keyRelease(KeyEvent.VK_CONTROL);
			if(withShift)
				getRobot().keyRelease(KeyEvent.VK_SHIFT);
			System.out.println("【debug】按Key:"+getKey());
		} catch (AWTException e) {
			System.out.println("【!】无法获取Robot,部分键盘模拟指令无法执行!");
		}
		
	}
	
	private String getKey()
	{
		StringBuffer buffer=new StringBuffer();
		if(withCtrl)
			buffer.append("Ctrl");
		if(withShift)
		{
			if(buffer.length()>0)
				buffer.append("+");
			buffer.append("Shift");
		}
		if(withAlt)
		{
			if(buffer.length()>0)
				buffer.append("+");
			buffer.append("Alt");
		}
		if(buffer.length()>0)
			buffer.append("+");
		buffer.append(KeyEvent.getKeyText(value));
		return buffer.toString();
	}

}
