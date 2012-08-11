package yon.autoupdate.net;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import yon.autoupdate.debug.Logger;

public class ConnectionManager {
	
	private static ConnectionManager instance=null;
	
	public static ConnectionManager getInstance(){
		if(instance==null)
			instance=new ConnectionManager();
		return instance;
	}

	public void setLogger(Logger logger) {
		TCPServer.getInstance().setLogger(logger);
	}
	
	public static TCPServer getTCPServer(){
		return TCPServer.getInstance();
	}
	
	
	public String getLocalIP(){
		try {
			Enumeration<NetworkInterface> netInterfaces = NetworkInterface.getNetworkInterfaces();
			while (netInterfaces.hasMoreElements()) {
				NetworkInterface ni = (NetworkInterface) netInterfaces.nextElement();
				InetAddress ip = (InetAddress) ni.getInetAddresses().nextElement();
				if (!ip.isSiteLocalAddress() && !ip.isLoopbackAddress()
						&& ip.getHostAddress().indexOf(":") == -1) {
					System.out.println("Interface " + ni.getName()
							+ " seems to be InternetInterface. I'll take it...");
				} else {
					if(ip.getHostAddress().equals("127.0.0.1")==false)
					{
						return ip.getHostAddress();
					}
				}
			}
			return null;
		} catch (SocketException e) {
			e.printStackTrace();
			return null;
		}
	}

}
