package yon.autoupdate.net;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;

import yon.autoupdate.debug.Logger;

//Transmission Control Protocol 
public class TCPServer{
	
	public String getStateString(int state){
		switch(state)
		{
		case STATE_EMPTY:
			return "EMPTY";
		case STATE_SETUP:
			return "SETUP";
		case STATE_INIT:
			return "INIT";
		case STATE_ACCEPT:
			return "ACCEPT";
		case STATE_PROCESS:
			return "PROCESS";
		case STATE_CLOSE:
			return "CLOSE";
		default:
			return "UNKNOWN";
		}
	}
	
	public static final int STATE_EMPTY=1;
	public static final int STATE_SETUP=2;
	public static final int STATE_INIT=3;
	public static final int STATE_ACCEPT=4;
	public static final int STATE_PROCESS=5;
	public static final int STATE_CLOSE=6;
	
	private static TCPServer instance=null;
	
	public static TCPServer getInstance(){
		if(instance==null)
			instance=new TCPServer();
		return instance;
	}
	
	private ServerSocket serverSocket;
	private StreamProcessor processor;
	private Callback callback;
	private Logger logger;
	private int state;
	
	public class StateCallback implements Callback{

		@Override
		public void doFinish() {
			//logger.debug("doFinish\r\n");
			state=STATE_CLOSE;
		}
		
	}
	
	public TCPServer() {
		this.state=STATE_EMPTY;
		this.serverSocket=null;
		this.callback=new StateCallback();
	}

	public void setLogger(Logger logger) {
		this.logger = logger;
	}
	
	public void setProcessor(StreamProcessor processor) {
		this.processor = processor;
	}
	
	public void connect(String ip,int port,StreamProcessor processor,Callback callback){
		logger.info("开始连接"+ip+"["+port+"]...\r\n");
		TCPRunnable r=new TCPRunnable(ip, port, processor,callback);
		r.setLogger(logger);
		new Thread(r).start();
	}


	public void monitorOnce(final int port,final Callback callback){
		new Thread(){
			public void run() {
				do{
					try {
						switch(state)
						{
						case STATE_EMPTY:
							//logger.debug("STATE_EMPTY\r\n");
							state=STATE_SETUP;
							serverSocket = new ServerSocket(port);
							//logger.debug("serverSocket = new ServerSocket("+port+");\r\n");
							break;
						case STATE_SETUP:
							state=STATE_INIT;
							serverSocket.setSoTimeout(100);
							//logger.debug("serverSocket.setSoTimeout(500);\r\n");
							logger.info("开始监听"+port+"端口上的连接...\r\n");
							break;
						case STATE_INIT:
						case STATE_ACCEPT:
							state=STATE_ACCEPT;
							Socket socket=serverSocket.accept();
							//logger.debug("STATE_ACCEPT\r\n");
							TCPRunnable r=new TCPRunnable(socket, processor,TCPServer.this.callback);
							r.setLogger(logger);
							new Thread(r).start();
							state=STATE_PROCESS;
							break;
						case STATE_PROCESS:
							//logger.debug("STATE_PROCESS\r\n");
							Thread.sleep(100);
							break;
						case STATE_CLOSE:
							//logger.debug("STATE_CLOSE\r\n");
							serverSocket.close();
							serverSocket=null;
							logger.info("已关闭"+port+"端口上的监听！\r\n");
							state=STATE_EMPTY;
							callback.doFinish();
						}
						
					} catch (SocketTimeoutException e){
						switch(state)
						{
						case STATE_ACCEPT:
						case STATE_PROCESS:
						case STATE_CLOSE:
							//repeat accept
							//logger.debug("SocketTimeoutException\r\n");
							break;
						default:
							logger.error("ServerSocket failed,for SocketTimeoutException:"+e.getLocalizedMessage()+"("+getStateString(state)+")\r\n");
							state=STATE_EMPTY;
							break;
						}
					} catch (IOException e) {
						logger.error("ServerSocket failed,for IOException:"+e.getLocalizedMessage()+"("+getStateString(state)+")\r\n");
						state=STATE_EMPTY;
					} catch (InterruptedException e) {
						logger.error("ServerSocket failed,for InterruptedException:"+e.getLocalizedMessage()+"("+getStateString(state)+")\r\n");
						state=STATE_EMPTY;
					}  
				}while(state!=STATE_EMPTY);
				//logger.debug("out\r\n");
			}
		}.start();
	}
	
	public void close(){
		//logger.debug("close\r\n");
		if(state!=STATE_EMPTY)
			state=STATE_CLOSE;
		while(state!=STATE_EMPTY);
	}

}
