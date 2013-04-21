package core;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.LinkedList;
import java.util.List;



public class UDPClient extends Thread {
	
	public static final int PORT=6548;
	public static final char COMMAND_SEND='S';
	
	/*
     * �������� Socket �����ĳ�ʱֵ,Ĭ��Ϊ0
     * ѡ������ڽ�����������ǰ���ò�����Ч��
     * ���������ʱֵ,��������������,������ java.io.InterruptedIOException���ڴ������,�����ر� Socket��
     *  �������׽���(SocketImpl��DatagramSocketImpl)����Ч��
     *  
     *  ����Receiver���������,��ֵ����Ϊ0
    */ 

    private static int SO_TIMEOUT=500;				//ҪԤ�����
    
    private static int SO_SNDBUF=8192;				//���ͻ���
    private static boolean SO_REUSEADDR=false;		//��ַ����
    private static boolean SO_BROADCAST=true;		//��ַ�㲥

    public  static final int LOW_COST = 0x02;
    public  static final int HIGH_RELIABILITY = 0x04;
    public  static final int HIGHTEST_THROUGHPUT = 0x08;
    public  static final int MINIMUM_DELAY = 0x10;

    private int trafficType=MINIMUM_DELAY;				//ҪԤ�����
	
	private boolean runnable=true;
	private List<DatagramPacket> packetQueue;
	private DatagramSocket datagramSocket;
	
	public UDPClient() throws SocketException{
		datagramSocket=new DatagramSocket();
    	datagramSocket.setBroadcast(SO_BROADCAST);
    	datagramSocket.setSendBufferSize(SO_SNDBUF);
    	datagramSocket.setReuseAddress(SO_REUSEADDR);
    	datagramSocket.setSoTimeout(SO_TIMEOUT);
    	datagramSocket.setTrafficClass(trafficType);
    	this.packetQueue=new LinkedList<DatagramPacket>();
	}
	
	public void run() {
		DatagramPacket packet=null;
		while(runnable)
		{
			synchronized (this.packetQueue) {
				while(this.packetQueue.isEmpty())
				{
//					System.out.println("PacketQueue is Empty");
					this.packetQueue.notify();
					try {
						this.packetQueue.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					if(runnable==false)
						return;
				}
				packet=(DatagramPacket) this.packetQueue.remove(0);
				try {
					send(packet);
//					System.out.println("Have send packet"+packet);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	private void send(DatagramPacket packet) throws IOException
	{
		datagramSocket.send(packet);
	}
	
	
	public void send(byte[] data,String ip) throws UnknownHostException
	{
		synchronized (this.packetQueue) {
			this.packetQueue.add(new DatagramPacket(data,data.length,InetAddress.getByName(ip),PORT));
			this.packetQueue.notify();
		}
	}
	
	public static String getLocalIP() throws UnknownHostException
	{
		return InetAddress.getLocalHost().getHostAddress();
	}
	
	public static String getIpPrefix(String ip)
	{
		if(ip==null)
			return null;
		int index=-1;
		for(int i=ip.length()-1;i>0;i--)
		{
			if(ip.charAt(i)=='.')
			{
				index=i;
				break;
			}
		}
		if(index==-1)
			return null;
		return ip.substring(0,index)+".";
	}
	
	public void close()
	{
		this.runnable=false;
		synchronized (this.packetQueue) {
			this.packetQueue.notify();
		}
		datagramSocket.close();
		System.out.println("UDPClient close");
	}
}
