package beta03;

import java.io.File;
import java.io.IOException;
import java.util.LinkedList;
import java.util.List;

import org.apache.xmlbeans.SchemaType;
import org.apache.xmlbeans.XmlException;

import script.SystemDocument;
import script.CommandDocument.Command;
import script.CommandDocument.Command.Type;
import script.CommanderDocument.Commander;
import script.CommanderDocument.Commander.Mode;

public class Config {
	
	private static final String FILE_PATH = "script.xml";
	
	/**
	 * µº»Î
	 * @return
	 * @throws XmlException
	 * @throws IOException
	 */
	public static List<beta03.Commander> load() throws XmlException, IOException {
		File file = new File(FILE_PATH);
		SystemDocument doc=SystemDocument.Factory.parse(file);
		List<beta03.Commander> commanders=new LinkedList<beta03.Commander>();
		for(Commander commander:doc.getSystem().getCommanderArray())
		{
			switch(commander.getMode().intValue())
			{
			case Mode.INT_BUNCH:
				commanders.add(loadBunchCommander(commander));
				break;
			case Mode.INT_LOOP:
				commanders.add(loadLoopCommander(commander));
				break;
			case Mode.INT_TIMER:
				commanders.add(loadTimerCommander(commander));
				break;
			}
		}
		return commanders;
	}
	
	private static LoopCommander loadLoopCommander(Commander commander)
	{
		LoopCommander loopCommander=new LoopCommander();
		loopCommander.setId(commander.getId());
		loopCommander.setCommands(loadCommands(commander.getCommandArray()));
		return loopCommander;
	}
	
	private static TimerCommander loadTimerCommander(Commander commander)
	{
		TimerCommander timerCommander=new TimerCommander();
		timerCommander.setId(commander.getId());
		timerCommander.setCommands(loadCommands(commander.getCommandArray()));
		
		return timerCommander;
	}
	
	private static BunchCommander loadBunchCommander(Commander commander)
	{
		BunchCommander bunchCommander=new BunchCommander();
		bunchCommander.setId(commander.getId());
		bunchCommander.setCommands(loadCommands(commander.getCommandArray()));
		return bunchCommander;
	}
	
	private static List<beta03.Command> loadCommands(Command[] commands)
	{
		for(Command command:commands)
		{
			switch(command.getType().intValue())
			{
			case Type.INT_CALL:
				break;
			case Type.INT_CHECK_COLOR:
				break;
			case Type.INT_KEY:
				break;
			case Type.INT_LEFT_MOUSE:
				break;
			case Type.INT_RIGHT_MOUSE:
				break;
			case Type.INT_SEND_UDP_MSG:
				break;
			case Type.INT_SHUT_DOWN:
				break;
			}
		}
		return null;
	}
	
}
