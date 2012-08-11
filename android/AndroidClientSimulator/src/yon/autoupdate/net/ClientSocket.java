package yon.autoupdate.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketTimeoutException;

import yon.autoupdate.debug.Logger;

public class ClientSocket implements Runnable {
	
	private String ip;
	private int port;
	private int timeout;
	private Socket socket;
	private StreamProcessor processor;
	private Callback callback;
	
	private static Logger logger;

	public static void setLogger(Logger logger) {
		ClientSocket.logger = logger;
	}
	
	private ClientSocket(String ip,int port,int timeout,StreamProcessor processor,Callback callback){
		this.ip=ip;
		this.port=port;
		this.timeout=timeout;
		this.socket=null;
		this.processor=processor;
		this.callback=callback;
	}
	
	public static void connect(String ip,int port,int timeout,StreamProcessor processor,Callback callback){
		logger.debug("connect:"+ip+","+port);
		new Thread(new ClientSocket(ip,port,timeout,processor,callback)).start();
	}

	@Override
	public void run() {
		try {
			if(this.socket==null)
			{
				this.socket=new Socket();
				SocketAddress remoteAddress=new InetSocketAddress(ip, port);
				this.socket.connect(remoteAddress, timeout);
				this.socket.setSoTimeout(timeout);
			}
			InputStream is = socket.getInputStream();
			OutputStream os = socket.getOutputStream();
			if(processor!=null)
				processor.process(is, os);
			//the close of InputStream and OutputStream does nothing
			is.close();
			os.close();
			socket.close();
			if(callback!=null)
				callback.doCompleted();
		} catch (SocketTimeoutException e) {
			logger.error("timeout:"+e.getLocalizedMessage()+"\r\n");
			if(callback!=null)
				callback.doTimeout();
		} catch (IOException e) {
			logger.error("failed:"+e.getLocalizedMessage()+"\r\n");
			if(callback!=null)
				callback.doFailed();
		}
	}

}
