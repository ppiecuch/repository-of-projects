package yon.autoupdate.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import yon.autoupdate.net.BaseStreamProcessor;

public class AutoupdateStreamProcessor extends BaseStreamProcessor {

	@Override
	public void process(InputStream is, OutputStream os) {
		DataInputStream dis=new DataInputStream(is);
		DataOutputStream dos=new DataOutputStream(os);
		try {
			int intValue=dis.readInt();
			logger.debug("receive "+intValue+" int!\r\n");
			dos.close();
			dis.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}