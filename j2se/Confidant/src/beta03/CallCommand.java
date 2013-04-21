package beta03;

import java.util.List;

public class CallCommand extends Command {
	
	private String value;
	private Commander commander;
	private List<Commander> commanders;

	public void perform() {
		if(getRefCommander()!=null)
		{
			for(Command command:commander.getCommands())
			{
				if(command.getDelay()>0)
				{
					try {
						Thread.sleep(command.getDelay());
					} catch (InterruptedException e) {
						System.out.println("¡¾!¡¿CallÖ¸Áî´®ÑÓ³Ù±»ÖÐ¶Ï");
					}
				}
				command.perform();
			}
			System.out.println("¡¾debug¡¿Call:"+value+"¿éÖ´ÐÐÍê±Ï");
		}
	}
	
	public Commander getRefCommander() {
		if(commander==null)
		{
			for(Commander c:commanders)
			{
				if(value!=null&&value.length()>0&&c.getId().equals(value))
					commander=c;
			}
		}
		return commander;
	}
	
	public void setCommanders(List<Commander> commanders) {
		this.commanders = commanders;
	}
	
	public void setValue(String value) {
		this.value = value;
	}

}
