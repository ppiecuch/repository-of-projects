package com.pixcoo.msg;

import java.util.Vector;

/**
 * 消息线程
 * 说明:
 * 1、支持对池添加消息与消费消息
 * 2、支持暂停执行（暂停时仍可向池添加消息）与恢复执行
 * 3、两个情形会导致线程进入暂停状态：调用pause方法和消息池为空时
 * 4、此类的成员方法没有对线程已死状态进行过滤，所以如果调用者不确定线程
 * 是否已死时，请使用isAlive()方法预先判断
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
	 * 向消息池中添加消息
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
	 * 暂停执行,但可向池添加消息
	 */
	public void pause()
	{
		this.runnable=false;
		System.out.println("pause:"+System.currentTimeMillis());
	}
	
	/**
	 * 恢复执行
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
	 * 关闭
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
			//对消息进行处理
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
	 * 判断池是否为空,可以根据业务需求重写(当池为空时,线程进入等待状态)
	 * @return
	 */
	private boolean isPoolEmpty()
	{
		return pool.isEmpty();
	}
	
	/**
	 * 从池中获取消息,可以根据自己需要实现
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
