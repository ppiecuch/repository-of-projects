package yon.autoupdate.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import yon.autoupdate.net.BaseStreamProcessor;

public class AutoupdateStreamProcessor extends BaseStreamProcessor {
	
	public static final int NEED_UPDATE_APK=1;

	@Override
	public void process(InputStream is, OutputStream os) {
		DataInputStream dis=new DataInputStream(is);
		DataOutputStream dos=new DataOutputStream(os);
		try {
			int intValue=dis.readInt();
			switch(intValue)
			{
			case NEED_UPDATE_APK:
				try {
					Thread.sleep(3000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				int versionCode=dis.readInt();
				int currentVersion=ExtractAndroidManifest.getVersionCode();
				boolean needUpdate=versionCode<currentVersion;
				dos.writeBoolean(needUpdate);
				logger.info("NEED_UPDATE_APK(version:"+versionCode+"):"+needUpdate+"\r\n");
				break;
			}
			dos.close();
			dis.close();
		} catch (IOException e) {
			logger.error(e.getMessage());
		}
		
	}

}