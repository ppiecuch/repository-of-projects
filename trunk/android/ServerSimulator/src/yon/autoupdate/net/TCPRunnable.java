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

public class TCPRunnable implements Runnable {
	
	private String ip;
	private int port;
	private Socket socket;
	private StreamProcessor processor;
	private Callback callback;
	private Logger logger;

	public void setLogger(Logger logger) {
		this.logger = logger;
	}
	
	public TCPRunnable(String ip,int port,StreamProcessor processor,Callback callback) {
		this.ip=ip;
		this.port=port;
		this.socket=null;
		this.processor=processor;
		this.callback=callback;
	}
	public TCPRunnable(Socket socket,StreamProcessor processor,Callback callback){
		this.socket=socket;
		this.processor=processor;
		this.callback=callback;
	}

	@Override
	public void run() {
		try {
			if(this.socket==null)
			{
				this.socket=new Socket();
				SocketAddress remoteAddress=new InetSocketAddress(InetAddress.getByName(ip), port);
				this.socket.connect(remoteAddress, 5000);
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
				callback.doFinish();
		} catch (SocketTimeoutException e) {
			logger.error("timeout:"+e.getLocalizedMessage()+"\r\n");
			if(callback!=null)
				callback.doFinish();
		} catch (IOException e) {
			logger.error(e.getLocalizedMessage()+"\r\n");
			if(callback!=null)
				callback.doFinish();
		}
	}

}
