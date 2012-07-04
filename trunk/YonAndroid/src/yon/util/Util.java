package yon.util;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigDecimal;
import java.util.Locale;

import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;
import android.os.StatFs;
import android.text.format.Formatter;

public class Util {
	
	public static String merge(String[] strs)
	{
		if(strs==null)
			return null;
		StringBuffer buffer=new StringBuffer();
		for(String s:strs)
		{
			if(buffer.length()==0)
				buffer.append(s);
			else
			{
				buffer.append(",");
				buffer.append(s);
			}
		}
		return buffer.toString();
	}
	
	public static void terminateProcess()
	{
		android.os.Process.killProcess(android.os.Process.myPid());
	}
	
	public static String getAvailableMemory(Context context) {// 获取android当前可用内存大小 

		ActivityManager am = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
		MemoryInfo mi = new MemoryInfo();
		am.getMemoryInfo(mi);//mi.availMem; 当前系统的可用内存 
		return Formatter.formatFileSize(context, mi.availMem);// 将获取的内存大小规格化 
	}
	
	public static String getTotalMemory(Context context) {
		String str1 = "/proc/meminfo";// 系统内存信息文件 
		String str2;
		String[] arrayOfString;
		long initial_memory = 0;

		try{
			FileReader localFileReader = new FileReader(str1);
			BufferedReader localBufferedReader = new BufferedReader(localFileReader, 8192);
			str2 = localBufferedReader.readLine();// 读取meminfo第一行，系统总内存大小 
			arrayOfString = str2.split("\\s+");
			initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位是KB，乘以1024转换为Byte 
			localBufferedReader.close();
		}catch (IOException e) {}
		return Formatter.formatFileSize(context, initial_memory);// Byte转换为KB或者MB，内存大小规格化 
	}
	
//	public static boolean isMeizuPhone()
//	{
//	    if ((instance.mPhoneplatform.equalsIgnoreCase("MEIZU MX")) || (instance.mPhoneplatform.equalsIgnoreCase("M030")) || (instance.mPhoneplatform.equalsIgnoreCase("M8")) || (instance.mPhoneplatform.equalsIgnoreCase("M9")) || (instance.mPhoneplatform.equalsIgnoreCase("meizu_m9")) || (Build.PRODUCT.startsWith("meizu")) || (Build.PRODUCT.startsWith("MEIZU")));
//	    for (int i = 1; ; i = 0)
//	      return i;
//	}

	public static String getSdCardPath() {
		return Environment.getExternalStorageDirectory().getAbsolutePath();
	}

	public static String getAPKFilePath(Context context) {
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
		if (Environment.getExternalStorageState().compareTo(
				Environment.MEDIA_MOUNTED) == 0) {
			return true;
		} else {
			return false;
		}
	}
	
	public static String getLanguage(){
		return Locale.getDefault().getLanguage();
	}

	public static boolean detectOpenGLES20(Context context) {
		ActivityManager am = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		return (info.reqGlEsVersion >= 0x20000);
	}

	public static String getExternalStoragePath() {
		// 获取SdCard状态
		String state = android.os.Environment.getExternalStorageState();
		// 判断SdCard是否存在并且是可用的
		if (android.os.Environment.MEDIA_MOUNTED.equals(state)) {
			if (android.os.Environment.getExternalStorageDirectory().canWrite()) {
				return android.os.Environment.getExternalStorageDirectory()
						.getPath();
			}
		}
		return null;
	}

	public static long getAvailableStore(String filePath) {
		// 取得sdcard文件路径
		StatFs statFs = new StatFs(filePath);
		// 获取block的SIZE
		long blocSize = statFs.getBlockSize();
		// 获取BLOCK数量
		// long totalBlocks = statFs.getBlockCount();
		// 可使用的Block的数量
		long availaBlock = statFs.getAvailableBlocks();
		// long total = totalBlocks * blocSize;
		long availableSpare = availaBlock * blocSize;
		return availableSpare;
	}

	public static String getFormatSize(double size) {
		double kiloByte = size / 1024;
		if (kiloByte < 1) {
			return size + "Byte(s)";
		}

		double megaByte = kiloByte / 1024;
		if (megaByte < 1) {
			BigDecimal result1 = new BigDecimal(Double.toString(kiloByte));
			return result1.setScale(2, BigDecimal.ROUND_HALF_UP)
					.toPlainString() + "KB";
		}

		double gigaByte = megaByte / 1024;
		if (gigaByte < 1) {
			BigDecimal result2 = new BigDecimal(Double.toString(megaByte));
			return result2.setScale(2, BigDecimal.ROUND_HALF_UP)
					.toPlainString() + "MB";
		}

		double teraBytes = gigaByte / 1024;
		if (teraBytes < 1) {
			BigDecimal result3 = new BigDecimal(Double.toString(gigaByte));
			return result3.setScale(2, BigDecimal.ROUND_HALF_UP)
					.toPlainString() + "GB";
		}
		BigDecimal result4 = new BigDecimal(teraBytes);
		return result4.setScale(2, BigDecimal.ROUND_HALF_UP).toPlainString()
				+ "TB";
	}

}
