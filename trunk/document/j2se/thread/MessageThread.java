package com.pixcoo.msg;

import java.util.Vector;

/**
 * ��Ϣ�߳�
 * ˵��:
 * 1��֧�ֶԳ������Ϣ��������Ϣ
 * 2��֧����ִͣ�У���ͣʱ�Կ���������Ϣ����ָ�ִ��
 * 3���������λᵼ���߳̽�����ͣ״̬������pause��������Ϣ��Ϊ��ʱ
 * 4������ĳ�Ա����û�ж��߳�����״̬���й��ˣ�������������߲�ȷ���߳�
 * �Ƿ�����ʱ����ʹ��isAlive()����Ԥ���ж�
 * @author Micheal
 *
 */
public class MessageThread extends Thread {
	
	private Vector<Object> pool;
	private static MessageThread instance;
	private boolean runnable;
	private boolean willClose;
	
	private MessageThread() {
		this.pool=new Vector<Object>();
		this.runnable=true;
		this.willClose=false;
	}
	
	/**
	 * ����Ϣ���������Ϣ
	 * @param obj
	 */
	public void put(Object obj)
	{
		synchronized(pool)
		{
			pool.add(obj);
			pool.notify();
			System.out.println("put:"+obj+","+System.currentTimeMillis());
		}
	}
	
	/**
	 * ��ִͣ��,������������Ϣ
	 */
	public void pause()
	{
		this.runnable=false;
		System.out.println("pause:"+System.currentTimeMillis());
	}
	
	/**
	 * �ָ�ִ��
	 */
	public void regain()
	{
		this.runnable=true;
		synchronized(pool)
		{
			this.pool.notify();
		}
		System.out.println("regain:"+System.currentTimeMillis());
	}
	
	/**
	 * �ر�
	 */
	public void close()
	{
		this.willClose=true;
		synchronized(pool)
		{
			this.pool.notify();
		}
	}
	
	
	
	@Override
	public void run() {
		Label:while(true)
		{
			if(willClose)break;
			Object obj;
			synchronized(pool)
			{
				while(isPoolEmpty()||runnable==false)
				{
					pool.notify();
					try {
						System.out.println("wait:"+System.currentTimeMillis());
						pool.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					if(willClose)break Label;
				}
				
				obj=getMessage();
				pool.notify();
			}
			//����Ϣ���д���
			System.out.println("do:"+obj+","+System.currentTimeMillis());
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		synchronized(pool){
			pool.clear();
		}
		this.pool=null;
		System.out.println("close:"+System.currentTimeMillis());
	}
	
	/**
	 * �жϳ��Ƿ�Ϊ��,���Ը���ҵ��������д(����Ϊ��ʱ,�߳̽���ȴ�״̬)
	 * @return
	 */
	private boolean isPoolEmpty()
	{
		return pool.isEmpty();
	}
	
	/**
	 * �ӳ��л�ȡ��Ϣ,���Ը����Լ���Ҫʵ��
	 * @return
	 */
	private Object getMessage()
	{
		return pool.remove(0);
	}
	
	public static MessageThread getInstance()
	{
		if(instance==null)
			instance=new MessageThread();
		return instance;
	}
	
	public static void main(String[] args) throws InterruptedException
	{
		MessageThread.getInstance().start();
		MessageThread.getInstance().put(1);
		MessageThread.getInstance().put(2);
		MessageThread.getInstance().put(3);
		
		MessageThread.getInstance().pause();
		MessageThread.getInstance().put(4);
		MessageThread.getInstance().put(5);
		System.out.println(MessageThread.getInstance().isAlive());
		MessageThread.getInstance().close();
		Thread.sleep(3000);
		if(MessageThread.getInstance().isAlive())
			MessageThread.getInstance().regain();
		Thread.sleep(1000);
		if(MessageThread.getInstance().isAlive())
			MessageThread.getInstance().close();
	}

}
