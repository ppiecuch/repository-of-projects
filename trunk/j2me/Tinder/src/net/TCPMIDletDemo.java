package net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.ServerSocketConnection;
import javax.microedition.io.SocketConnection;
import javax.microedition.io.file.FileConnection;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class TCPMIDletDemo extends MIDlet implements CommandListener {
	
	public static final int PORT=5487;

	protected void destroyApp(boolean unconditional){

	}

	protected void pauseApp() {

	}

	protected void startApp() throws MIDletStateChangeException {
		TCPServer server=new TCPServer(){

			public void doSocket(final SocketConnection sc) {
				new Thread(){
					public void run() {
						//接收文件
						FileConnection fc=null;
						OutputStream os=null;
						InputStream is=null;
						try {
							fc=(FileConnection)Connector.open("file://:");
							os=fc.openOutputStream();
							is=sc.openInputStream();
							while(is.available()>0)
							{
								os.write(is.read());
							}
							is.close();
							os.close();
							fc.close();
							sc.close();
						} catch (IOException e) {
							e.printStackTrace();
						} 
					}
				}.start();
			}
			
		};
		server.start();
		
		//发送文件
		new Thread(){
			public void run() {
				String ip="127.0.0.1";
				FileConnection fc=null;
				SocketConnection sc=null;
				OutputStream os=null;
				InputStream is=null;
				try {
					fc=(FileConnection)Connector.open("file://:");
					sc=TCPClient.connect(ip);
					os=sc.openOutputStream();
					is=fc.openInputStream();
					while(is.available()>0)
					{
						os.write(is.read());
					}
					is.close();
					os.close();
					fc.close();
					sc.close();
				} catch (IOException e) {
					e.printStackTrace();
				} 
			}
		}.start();
	}

	public void commandAction(Command c, Displayable s) {
		
	}

}
abstract class TCPServer extends Thread{
	
	protected ServerSocketConnection scn;
	protected boolean runnable=true;
	
	public TCPServer() {
		try {
			scn = (ServerSocketConnection) Connector.open("socket://:"+TCPMIDletDemo.PORT);
		} catch (IOException e) {
			scn=null;
			e.printStackTrace();
		}
	}
	
	public void run() {
		while(runnable&&scn!=null)
		{
			try {
				SocketConnection sc = (SocketConnection) scn.acceptAndOpen();
				doSocket(sc);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public abstract void doSocket(final SocketConnection sc);
	
}

class TCPClient{
	
	public static SocketConnection connect(String ip) throws IOException
	{
		 return (SocketConnection) Connector.open("socket://"+ip+":"+TCPMIDletDemo.PORT);
	}
}