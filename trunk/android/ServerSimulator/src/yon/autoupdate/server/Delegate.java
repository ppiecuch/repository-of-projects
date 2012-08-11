package yon.autoupdate.server;

import javax.swing.JTextArea;

import yon.autoupdate.core.BaseState;
import yon.autoupdate.core.StateManager;
import yon.autoupdate.debug.TextAreaLogger;
import yon.autoupdate.net.Callback;
import yon.autoupdate.net.ConnectionManager;

public class Delegate {
	
	private final BaseState OPEN;
	private final BaseState CLOSE;
	private final TextAreaLogger logger;
	private final AutoupdateStreamProcessor processor;
	
	public Delegate(){
		this.OPEN=new OpenState();
		this.CLOSE=new CloseState();
		
		this.logger=new TextAreaLogger();
		this.OPEN.setLogger(this.logger);
		this.CLOSE.setLogger(this.logger);
		
		this.processor=new AutoupdateStreamProcessor();
		
		
	}
	
	public void setOutput(JTextArea textArea)
	{
		this.logger.setOutput(textArea);
		this.processor.setLogger(logger);
		ConnectionManager.getInstance().setLogger(logger);
	}
	
	public void setup(Callback callback){
		ConnectionManager.getTCPServer().setProcessor(processor);
		ConnectionManager.getTCPServer().monitorOnce(8080,callback);
		StateManager.getInstance().setState(OPEN);
	}
	
	public void close(){
		ConnectionManager.getTCPServer().close();
		StateManager.getInstance().setState(CLOSE);
	}
	
	private static Delegate instance=null;
	
	public static Delegate getInstance(){
		if(instance==null)
			instance=new Delegate();
		return instance;
	}
}
