package yon.util;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;

public class Util {
	
	public static String getSdCardPath(){
		return Environment.getExternalStorageDirectory().getAbsolutePath();
	}
	
	public static String getAPKFilePath(Context context){
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = context.getPackageManager();
		try {
		    appInfo = packMgmr.getApplicationInfo(context.getPackageName(), 0);
	    } catch (NameNotFoundException e) {
		    e.printStackTrace();
		    throw new RuntimeException("Unable to locate assets, aborting...");
	    }
		return appInfo.sourceDir;
	}
	
	public static boolean isSdCardAvailable() {
        if (Environment.getExternalStorageState().compareTo(Environment.MEDIA_MOUNTED)==0) {
            return true;
        } else {
            return false;
        }
    }
	
	public static boolean detectOpenGLES20(Context context) {  
    	ActivityManager am =  (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);  
    	ConfigurationInfo info = am.getDeviceConfigurationInfo();  
    	return (info.reqGlEsVersion >= 0x20000);  
    }

}
