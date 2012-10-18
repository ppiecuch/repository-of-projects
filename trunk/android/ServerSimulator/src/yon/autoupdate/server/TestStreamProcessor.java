package yon.autoupdate.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import yon.autoupdate.net.BaseStreamProcessor;

/**
 * @author hongzhibiao
 * @email babala_234@163.com
 */
public class TestStreamProcessor extends BaseStreamProcessor {

	@Override
	public void process(InputStream is, OutputStream os) {
		/*DataInputStream dis=new DataInputStream(is);
		DataOutputStream dos=new DataOutputStream(os);
		try {
			String str=dis.readUTF();
			logger.info("read:"+str+"\n");
			dos.close();
			dis.close();
		} catch (IOException e) {
			logger.error(e.toString()+"\n");
		}*/
		byte buffer[]=new byte[11];
		try {
			for(int i=0;i<20;++i)
			{
				int num=is.read(buffer);
				String str=new String(buffer,0,num,"utf8");
				logger.info("read---num:"+num+",str:"+str+"\n");
				
				Thread.sleep(1000);
			}
		} catch (IOException e) {
			logger.error(e.toString()+"\n");
		} catch (InterruptedException e) {
			logger.error(e.toString()+"\n");
		}

	}

}
