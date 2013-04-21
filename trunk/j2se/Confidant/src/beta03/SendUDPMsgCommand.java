package beta03;

import java.io.UnsupportedEncodingException;
import java.net.SocketException;
import java.net.UnknownHostException;

import core.UDPClient;

public class SendUDPMsgCommand extends Command {
	
	private String ip;
	private String value;
	private static UDPClient udpClient;

	public void perform() {
		String data=UDPClient.COMMAND_SEND+"@"+value+"@"+1000;
		System.out.println("��debug��������Ϣ��IP:"+ip);
		try {
			getUDPClient().send(data.getBytes("UTF-8"),ip);
		} catch (UnknownHostException e) {
			System.out.println("��debug�����ͳ���:UnknownHostException");
		} catch (UnsupportedEncodingException e) {
			System.out.println("��debug�����ͳ���:UnsupportedEncodingException");
		}

	}
	
	private static UDPClient getUDPClient()
	{
		if(udpClient==null)
		{
			try {
				udpClient=new UDPClient();
				udpClient.start();
			} catch (SocketException e) {
				e.printStackTrace();
			}
		}
		return udpClient;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}
	
	

}
