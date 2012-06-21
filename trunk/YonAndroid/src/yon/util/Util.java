package yon.util;

import java.math.BigDecimal;
import java.util.Locale;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;
import android.os.StatFs;

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
