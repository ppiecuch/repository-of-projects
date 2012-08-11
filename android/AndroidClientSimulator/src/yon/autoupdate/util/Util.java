package yon.autoupdate.util;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.DhcpInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.util.Log;

public class Util {
	
	public static boolean isNetworkAvailable(Context context) {
		ConnectivityManager conMan = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		State wifi = conMan.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState();
		State mobile = conMan.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState();
		
        if(mobile==State.CONNECTED||mobile==State.CONNECTING)
            return true;
        if(wifi==State.CONNECTED||wifi==State.CONNECTING)
            return true;
        return false;
	}
	
	public static boolean isSDCardAvailable() {
		if (Environment.getExternalStorageState().compareTo(Environment.MEDIA_MOUNTED) == 0) {
			return true;
		} else {
			return false;
		}
	}
	
	public static int getVersionCode(Context context){
		try {
			return context.getPackageManager().getPackageInfo(context.getPackageName(),PackageManager.GET_SIGNATURES).versionCode;
		} catch (NameNotFoundException e) {
			return -1;
		}
	}
	
	public static String getVersionName(Context context){
		try {
			return context.getPackageManager().getPackageInfo(context.getPackageName(),PackageManager.GET_SIGNATURES).versionName;
		} catch (NameNotFoundException e) {
			return null;
		}
	}
	
	public static String getLocalMacAddress(Context context)       {                          
		WifiManager wifi = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);            
		WifiInfo info = wifi.getConnectionInfo();            
		return info.getMacAddress();        
	}
	
	public static DhcpInfo getDHCPInfo(Context context)       {                          
		WifiManager wifi = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);            
		return wifi.getDhcpInfo(); 
	}
	
	public static String getLocalIP(){
		try {
			Enumeration<NetworkInterface> netInterfaces = NetworkInterface.getNetworkInterfaces();
			while (netInterfaces.hasMoreElements()) {
				NetworkInterface ni = (NetworkInterface) netInterfaces.nextElement();
				InetAddress ip = (InetAddress) ni.getInetAddresses().nextElement();
				if (!ip.isSiteLocalAddress() && !ip.isLoopbackAddress()&& ip.getHostAddress().indexOf(":") == -1) {
					Log.e("Util","Interface " + ni.getName() + " seems to be InternetInterface. I'll take it...");
				} else {
					if(ip.getHostAddress().equals("127.0.0.1")==false)
					{
						return ip.getHostAddress();
					}
				}
			}
			return null;
		} catch (SocketException e) {
			Log.e("Util",e.getLocalizedMessage());
			return null;
		}
	}

}
