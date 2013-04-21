package beta03;

import java.io.File;
import java.io.IOException;
import java.util.LinkedList;
import java.util.List;

import org.apache.xmlbeans.XmlException;

import script.SystemDocument;
import script.CommandDocument.Command;
import script.CommandDocument.Command.Type;
import script.CommanderDocument.Commander;
import script.CommanderDocument.Commander.Mode;

public class Config {
	
	private static final String FILE_PATH = "script.xml";
	private static List<beta03.Commander> commanders;
	
	/**
	 * µº»Î
	 * @return
	 * @throws XmlException
	 * @throws IOException
	 */
	public static List<beta03.Commander> load() throws XmlException, IOException {
		File file = new File(FILE_PATH);
		SystemDocument doc=SystemDocument.Factory.parse(file);
		commanders=new LinkedList<beta03.Commander>();
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
		loopCommander.setMode(beta03.Commander.MODE_LOOP);
		loopCommander.setCommands(loadCommands(commander.getCommandArray()));
		if(commander.getTime()!=null)
			loopCommander.setTime(Long.parseLong(commander.getTime()));
		else
			loopCommander.setTime(0);
		return loopCommander;
	}
	
	private static TimerCommander loadTimerCommander(Commander commander)
	{
		TimerCommander timerCommander=new TimerCommander();
		timerCommander.setId(commander.getId());
		timerCommander.setMode(beta03.Commander.MODE_TIMER);
		timerCommander.setCommands(loadCommands(commander.getCommandArray()));
		
		return timerCommander;
	}
	
	private static BunchCommander loadBunchCommander(Commander commander)
	{
		BunchCommander bunchCommander=new BunchCommander();
		bunchCommander.setId(commander.getId());
		bunchCommander.setMode(beta03.Commander.MODE_BUNCH);
		bunchCommander.setCommands(loadCommands(commander.getCommandArray()));
		return bunchCommander;
	}
	
	private static List<beta03.Command> loadCommands(Command[] commands)
	{
		List<beta03.Command> list=new LinkedList<beta03.Command>();
		for(Command command:commands)
		{
			switch(command.getType().intValue())
			{
			case Type.INT_CALL:
				list.add(loadCallCommand(command));
				break;
			case Type.INT_CHECK_COLOR:
				list.add(loadCheckColorCommand(command));
				break;
			case Type.INT_KEY:
				list.add(loadKeyCommand(command));
				break;
			case Type.INT_LEFT_MOUSE:
				list.add(loadMouseCommand(command));
				break;
			case Type.INT_RIGHT_MOUSE:
				list.add(loadMouseCommand(command));
				break;
			case Type.INT_SEND_UDP_MSG:
				list.add(loadSendUDPMsgCommand(command));
				break;
			case Type.INT_SHUT_DOWN:
				break;
			}
		}
		return list;
	}
	
	private static CallCommand loadCallCommand(Command command){
		CallCommand callCommand=new CallCommand();
		callCommand.setCommanders(commanders);
		callCommand.setType(beta03.Command.TYPE_CALL);
		callCommand.setDelay(command.getDelay());
		callCommand.setValue(command.getValue());
		return callCommand;
	}
	
	private static SendUDPMsgCommand loadSendUDPMsgCommand(Command command)
	{
		SendUDPMsgCommand sendUDPMsgCommand=new SendUDPMsgCommand();
		sendUDPMsgCommand.setType(beta03.Command.TYPE_SEND_UDP_MSG);
		sendUDPMsgCommand.setDelay(command.getDelay());
		sendUDPMsgCommand.setValue(command.getValue());
		sendUDPMsgCommand.setIp(command.getIp());
		return sendUDPMsgCommand;
	}
	
	private static KeyCommand loadKeyCommand(Command command)
	{
		KeyCommand keyCommand=new KeyCommand();
		keyCommand.setType(beta03.Command.TYPE_KEY);
		keyCommand.setDelay(command.getDelay());
		keyCommand.setValue(Integer.valueOf(command.getValue()));
		keyCommand.setWithAlt(command.getAlt());
		keyCommand.setWithCtrl(command.getCtrl());
		keyCommand.setWithShift(command.getShift());
		return keyCommand;
	}
	
	private static MouseCommand loadMouseCommand(Command command)
	{
		MouseCommand mouseCommand=new MouseCommand();
		switch(command.getType().intValue())
		{
		case Type.INT_LEFT_MOUSE:
			mouseCommand.setType(beta03.Command.TYPE_LEFT_MOUSE);
			break;
		case Type.INT_RIGHT_MOUSE:
			mouseCommand.setType(beta03.Command.TYPE_RIGHT_MOUSE);
			break;
		}
		mouseCommand.setX(command.getX());
		mouseCommand.setY(command.getY());
		mouseCommand.setDelay(command.getDelay());
		return mouseCommand;
		
	}
	
	private static CheckColorCommand loadCheckColorCommand(Command command)
	{
		CheckColorCommand checkColorCommand=new CheckColorCommand();
		checkColorCommand.setDelay(command.getDelay());
		checkColorCommand.setType(beta03.Command.TYPE_CHECK_COLOR);
		checkColorCommand.setValue(command.getValue());
		checkColorCommand.setX(command.getX());
		checkColorCommand.setY(command.getY());
		checkColorCommand.setFalseRef(command.getFalseRef());
		checkColorCommand.setTrueRef(command.getTrueRef());
		checkColorCommand.setCommanders(commanders);
		return checkColorCommand;
	}
	
	
}
