package yon.autoupdate.net;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.util.Log;

public class ConnectionManager {
	
	private static ConnectionManager instance=null;
	
	public static ConnectionManager getInstance(){
		if(instance==null)
			instance=new ConnectionManager();
		return instance;
	}
	
	public String getLocalIP(){
		try {
			Enumeration<NetworkInterface> netInterfaces = NetworkInterface.getNetworkInterfaces();
			while (netInterfaces.hasMoreElements()) {
				NetworkInterface ni = (NetworkInterface) netInterfaces.nextElement();
				InetAddress ip = (InetAddress) ni.getInetAddresses().nextElement();
				if (!ip.isSiteLocalAddress() && !ip.isLoopbackAddress()&& ip.getHostAddress().indexOf(":") == -1) {
					Log.e(getClass().getName(),"Interface " + ni.getName() + " seems to be InternetInterface. I'll take it...");
				} else {
					if(ip.getHostAddress().equals("127.0.0.1")==false)
					{
						return ip.getHostAddress();
					}
				}
			}
			return null;
		} catch (SocketException e) {
			Log.e(getClass().getName(),e.getLocalizedMessage());
			return null;
		}
	}

}
