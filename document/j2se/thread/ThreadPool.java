package com.pixcoo.monitor.common;



import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

import org.apache.log4j.Logger;

/**
 * 线程池
 * 说明:
 * 1、支持对池添加对象,并让池里的多线程对之进行处理;
 * 2、支持暂停执行（暂停时仍可向池添加对象）与恢复执行
 * 3、两个情形会导致线程进入等待状态：调用pause方法和消息池为空时
 * 4、此类的成员方法没有对线程已死状态进行过滤，所以如果调用者不确定线程
 * 是否已死时，请使用isAlive()方法预先判断
 * @author Micheal
 *
 */
public class ThreadPool<T extends Processible> {
	
	private Vector<T> pool;
	private Set<PoolThread> threads;
	private boolean runnable;
	private boolean willClose; 
	private int threadNum;
	
	private static Logger log = Logger.getLogger(ThreadPool.class);
	
	public ThreadPool(){}
	
	public ThreadPool(int num) {
		this.threadNum=num;
	}
	
	public void setThreadNum(int num) {
		this.threadNum = num;
	}

	public void init()
	{
		threads=new HashSet<PoolThread>();
		for(int i=0;i<threadNum;i++)
			threads.add(new PoolThread("thread-"+i));
		this.pool=new Vector<T>();
		this.runnable=true;
		this.willClose=false;
	}

	public void start()
	{
		for(PoolThread t:threads)
		{
			t.start();
		}
		log.info("【√】ThreadPool start");
	}
	
	/**
	 * 向池中添加对象
	 * @param obj
	 */
	public void put(T t)
	{
		long start=System.currentTimeMillis();
		log.debug("Start putting "+t+" into ThreadPool");
		synchronized(pool)
		{
			pool.add(t);
			//只有非暂停状态才唤醒线程
			if(this.runnable)
			{
				pool.notifyAll();
			}
		}
		long end=System.currentTimeMillis();
		log.debug("End putting "+t+" into ThreadPool,use time:"+(end-start)+"ms");
	}
	
	/**
	 * 向池中添加对象
	 * @param obj
	 */
	public void putAll(Collection<T> c)
	{
		long start=System.currentTimeMillis();
		log.debug("Start putting all("+c.size()+") into ThreadPool");
		synchronized(pool)
		{
			pool.addAll(c);
			//只有非暂停状态才唤醒线程
			if(this.runnable)
			{
				pool.notifyAll();
			}
		}
		long end=System.currentTimeMillis();
		log.debug("End putting all("+c.size()+") into ThreadPool,use time:"+(end-start)+"ms");
	}
	
	/**
	 * 暂停执行,但可向池添加对象
	 */
	public void pause()
	{
		this.runnable=false;
		log.info("【||】ThreadPool pause");
	}
	
	/**
	 * 恢复执行
	 */
	public void regain()
	{
		this.runnable=true;
		synchronized(pool)
		{
			this.pool.notifyAll();
		}
		log.info("【√】ThreadPool regain");
	}
	
	public boolean isAlive()
	{
		return pool!=null;
	}
	
	/**
	 * 关闭
	 */
	public void close()
	{
		long start=System.currentTimeMillis();
		log.debug("Start closing ThreadPool");
		this.willClose=true;
		synchronized(pool)
		{
			this.pool.notifyAll();
		}
		log.info("【◇】ThreadPool close");
		long end=System.currentTimeMillis();
		log.debug("End closing ThreadPool,use time:"+(end-start)+"ms");
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
	private T get()
	{
		return pool.remove(0);
	}
	
	class PoolThread extends Thread{
		
		public PoolThread(String name) {
			super(name);
		}
		
		@Override
		public void run() {
			Label:while(true)
			{
				if(willClose)break;
				Processible t;
				synchronized(pool)
				{
					while(isPoolEmpty()||runnable==false)
					{
						try {
							log.debug(this+" wait");
							pool.wait();
							
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						if(willClose)break Label;
					}
					
					t=get();
					log.debug(this+" get:"+t);
				}
				//对对象进行处理
				try{
					t.process();
				}catch(Exception e){
					log.error(Util.wrap(e));
				}
			}
			//持有threads锁期间请求pool锁,可能出现死锁情况
			//但线程在持有pool锁期间内并没再持有threads锁(代码里根本没有这样的请求),所以不可能有死锁
			synchronized (threads) {
				threads.remove(this);
				if(threads.size()==0)
				{
					synchronized(pool){
						pool.clear();
					}
					pool=null;
				}
				log.debug("Close "+this+",threads.size:"+threads.size());
			}
		}
		
		@Override
		public String toString() {
			return getName();
		}
	}

	/**
	 * @param args
	 * @throws InterruptedException 
	 */
//	public static void main(String[] args) throws InterruptedException {
//		ThreadPool<Test> p=new ThreadPool<Test>(3);
//		p.start();
//		p.put(new Test("test1"));
//		p.put(new Test("test2"));
//		p.put(new Test("test3"));
//		
//		Thread.sleep(1);
//		p.pause();
//		System.out.println("pause");
//		p.put(new Test("test4"));
//		p.put(new Test("test5"));
//		p.put(new Test("test6"));
//		Thread.sleep(1500);
//		p.regain();
//		System.out.println("regain");
//		Thread.sleep(1500);
//		p.put(new Test("test7"));
//		if(p.isAlive())
//		{
//			p.put(new Test("test8"));
//			p.put(new Test("test9"));
//		}
//		p.close();
//		if(p.isAlive())
//		{
//			p.pause();
//		}
//		
//	}

}


