package beta03;

import java.util.List;

import core.TaskThread;

public abstract class Commander{
	
	public static final int MODE_LOOP=1;
	public static final int MODE_TIMER=2;
	public static final int MODE_BUNCH=3;
	
	
	protected TaskThread executive;
	protected List<Command> commands;
	protected String id;
	protected int mode;
	
	
	public List<Command> getCommands() {
		return commands;
	}
	public void setCommands(List<Command> commands) {
		this.commands = commands;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public int getMode() {
		return mode;
	}
	public void setMode(int mode) {
		this.mode = mode;
	}
	public abstract void start();
	public abstract void close();
	
	public void setExecutive(TaskThread executive) {
		this.executive = executive;
	}

}
