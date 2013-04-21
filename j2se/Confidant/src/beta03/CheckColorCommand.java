package beta03;

import java.awt.AWTException;
import java.util.List;

import core.Snapshot;


public class CheckColorCommand extends Command {
	
	private int x;
	private int y;
	private String value;
	private String trueRef;
	private String falseRef;
	private Commander trueRefCommander;
	private Commander falseRefCommander;
	private List<Commander> commanders;
	
	public void perform() {
		try {
			String color=Integer.toHexString(Snapshot.getScreenPixelColor(x, y));
			if(value.toLowerCase().indexOf(color)!=-1)
			{
				System.out.println("【debug】"+x+","+y+":"+color+"=="+value);
				if(getTrueRefCommander()!=null)
				{
					for(Command command:trueRefCommander.getCommands())
					{
						if(command.getDelay()>0)
						{
							try {
								Thread.sleep(command.getDelay());
							} catch (InterruptedException e) {
								System.out.println("【!】CheckColor指令串延迟被中断");
							}
						}
						command.perform();
					}
				}
				System.out.println("【debug】TrueRef块执行完毕");
			}
			else
			{
				System.out.println("【debug】"+x+","+y+":"+color+"!="+value);
				if(getFalseRefCommander()!=null)
				{
					for(Command command:falseRefCommander.getCommands())
					{
						if(command.getDelay()>0)
						{
							try {
								Thread.sleep(command.getDelay());
							} catch (InterruptedException e) {
								System.out.println("【!】CheckColor指令串延迟被中断");
							}
						}
						command.perform();
					}
				}
				System.out.println("【debug】FalseRef块执行完毕");
			}
		} catch (AWTException e) {
			System.out.println("【!】无法获取Robot,屏幕像素无法获取!");
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

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}

	public String getTrueRef() {
		return trueRef;
	}

	public void setTrueRef(String trueRef) {
		this.trueRef = trueRef;
	}

	public String getFalseRef() {
		return falseRef;
	}

	public void setFalseRef(String falseRef) {
		this.falseRef = falseRef;
	}

	public Commander getTrueRefCommander() {
		if(trueRefCommander==null)
		{
			for(Commander c:commanders)
			{
				if(trueRef!=null&&trueRef.length()>0&&c.getId().equals(trueRef))
					trueRefCommander=c;
			}
		}
		return trueRefCommander;
	}

	public Commander getFalseRefCommander() {
		if(falseRefCommander==null)
		{
			for(Commander c:commanders)
			{
				if(falseRef!=null&&falseRef.length()>0&&c.getId().equals(falseRef))
					falseRefCommander=c;
			}
		}
		return falseRefCommander;
	}

	public void setCommanders(List<Commander> commanders) {
		this.commanders = commanders;
	}
	
	

}
