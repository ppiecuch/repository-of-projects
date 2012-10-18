package yon.autoupdate.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;

import yon.autoupdate.net.BaseStreamProcessor;

/**
 * @author hongzhibiao
 * @email babala_234@163.com
 */
public class TestStreamProcessor extends BaseStreamProcessor {

	@Override
	public void process(InputStream is, OutputStream os) {
		try {
			String str=new String("helloÄãºÃ");
			for(int i=0;i<20;++i){
				os.write(str.getBytes("utf8"));
				logger.info("write---num:"+str.getBytes("utf8").length+",str:"+str+"\n");
			}
		} catch (UnsupportedEncodingException e) {
			logger.error(e.toString()+"\n");
		} catch (IOException e) {
			logger.error(e.toString()+"\n");
		}
		/*DataInputStream dis=new DataInputStream(is);
		DataOutputStream dos=new DataOutputStream(os);
		try {
			dos.writeUTF("helloÄãºÃ");
			dos.close();
			dis.close();
		} catch (IOException e) {
			logger.error(e.toString()+"\n");
		}*/
	}

}
