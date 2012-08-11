package yon.autoupdate.androidclientsimulator;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.Properties;

import yon.autoupdate.debug.ConsoleLogger;
import yon.autoupdate.debug.Logger;

import android.app.Application;
import android.os.Environment;
import android.util.Log;

public class GameApplication extends Application {

	public static final String TAG="GameApplication";
	
	private static final String CONFIG_FILE_NAME="config.properties";
	private static final String SD_DIR="syjt";
	
	private Properties properties = null;
	private Logger logger;
	
	@Override
	public void onCreate() {
		super.onCreate();
		
		this.logger=new ConsoleLogger();
		
		Log.d(TAG,"GameApplication");
		InputStream is=null;
		try {
			is = getAssets().open(CONFIG_FILE_NAME);
			this.properties=new Properties();
			this.properties.load(is);
			Log.d(TAG,getString("TEST"));
			Log.d(TAG,getString("DEMO"));
			Log.d(TAG,getGamePath());
		} catch (IOException e) {
			Log.e(TAG,e.getMessage());
		} finally {
			if(is!=null)
			{
				try {
					is.close();
					Log.d(TAG,"is.close()");
				} catch (IOException e) {}
			}
		}
	}
	
	public Logger getLogger(){
		return logger;
	}
	
	public String getString(String name)
	{
		try {
			return new String(properties.getProperty(name).getBytes("ISO-8859-1"),"UTF-8");
		} catch (UnsupportedEncodingException e) {
			return properties.getProperty(name);
		}
	}
	
	public Integer getInteger(String name)
	{
		String str=properties.getProperty(name);
		if(name==null)
			return null;
		return Integer.parseInt(str);
	}
	
	public String getGamePath()
	{
		File localFile = Environment.getExternalStorageDirectory();
		String str = localFile.getPath();
		return str + "/"+SD_DIR;
	}
}
