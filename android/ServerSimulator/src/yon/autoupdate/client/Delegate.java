package yon.autoupdate.client;

import javax.swing.JTextArea;

import yon.autoupdate.debug.TextAreaLogger;
import yon.autoupdate.net.Callback;
import yon.autoupdate.net.ConnectionManager;

public class Delegate {
	
	private final TextAreaLogger logger;
	
	public Delegate(){
		this.logger=new TextAreaLogger();
	}
	
	public void setOutput(JTextArea textArea)
	{
		this.logger.setOutput(textArea);
		ConnectionManager.getInstance().setLogger(logger);
	}
	
	public void connect(String ip,int port,Callback callback){
		//AutoupdateStreamProcessor processor=new AutoupdateStreamProcessor();
		TestStreamProcessor processor=new TestStreamProcessor();
		processor.setLogger(logger);
		ConnectionManager.getTCPServer().connect(ip, port, processor,callback);
	}
	
	public void close(){
	}
	
	private static Delegate instance=null;
	
	public static Delegate getInstance(){
		if(instance==null)
			instance=new Delegate();
		return instance;
	}
}
