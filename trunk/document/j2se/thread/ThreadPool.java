package com.pixcoo.monitor.common;



import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

import org.apache.log4j.Logger;

/**
 * �̳߳�
 * ˵��:
 * 1��֧�ֶԳ���Ӷ���,���ó���Ķ��̶߳�֮���д���;
 * 2��֧����ִͣ�У���ͣʱ�Կ������Ӷ�����ָ�ִ��
 * 3���������λᵼ���߳̽���ȴ�״̬������pause��������Ϣ��Ϊ��ʱ
 * 4������ĳ�Ա����û�ж��߳�����״̬���й��ˣ�������������߲�ȷ���߳�
 * �Ƿ�����ʱ����ʹ��isAlive()����Ԥ���ж�
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
		log.info("���̡�ThreadPool start");
	}
	
	/**
	 * �������Ӷ���
	 * @param obj
	 */
	public void put(T t)
	{
		long start=System.currentTimeMillis();
		log.debug("Start putting "+t+" into ThreadPool");
		synchronized(pool)
		{
			pool.add(t);
			//ֻ�з���ͣ״̬�Ż����߳�
			if(this.runnable)
			{
				pool.notifyAll();
			}
		}
		long end=System.currentTimeMillis();
		log.debug("End putting "+t+" into ThreadPool,use time:"+(end-start)+"ms");
	}
	
	/**
	 * �������Ӷ���
	 * @param obj
	 */
	public void putAll(Collection<T> c)
	{
		long start=System.currentTimeMillis();
		log.debug("Start putting all("+c.size()+") into ThreadPool");
		synchronized(pool)
		{
			pool.addAll(c);
			//ֻ�з���ͣ״̬�Ż����߳�
			if(this.runnable)
			{
				pool.notifyAll();
			}
		}
		long end=System.currentTimeMillis();
		log.debug("End putting all("+c.size()+") into ThreadPool,use time:"+(end-start)+"ms");
	}
	
	/**
	 * ��ִͣ��,���������Ӷ���
	 */
	public void pause()
	{
		this.runnable=false;
		log.info("��||��ThreadPool pause");
	}
	
	/**
	 * �ָ�ִ��
	 */
	public void regain()
	{
		this.runnable=true;
		synchronized(pool)
		{
			this.pool.notifyAll();
		}
		log.info("���̡�ThreadPool regain");
	}
	
	public boolean isAlive()
	{
		return pool!=null;
	}
	
	/**
	 * �ر�
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
		log.info("����ThreadPool close");
		long end=System.currentTimeMillis();
		log.debug("End closing ThreadPool,use time:"+(end-start)+"ms");
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
				//�Զ�����д���
				try{
					t.process();
				}catch(Exception e){
					log.error(Util.wrap(e));
				}
			}
			//����threads���ڼ�����pool��,���ܳ����������
			//���߳��ڳ���pool���ڼ��ڲ�û�ٳ���threads��(���������û������������),���Բ�����������
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


