package beta03;

import java.util.Date;

public class LoopCommander extends Commander implements Runnable{
	
	private Thread thread;
	private boolean runnable=true;
	private long time;		//ʱ����
	
	public LoopCommander() {
		thread=new Thread(this);
	}

	@Override
	public void close() {
//		taskThread.close();
		runnable=false;
		if(thread!=null)
			thread.interrupt();
		while(thread!=null&&thread.isAlive());
		
	}

	@Override
	public void start() {
		thread.start();
	}

	public void run() {
		int index=0;
		long useTime=0;
		long startTime=new Date().getTime();
		long endTime;
		while(runnable)
		{
			Command command=commands.get(index);
			if(command.getDelay()>0)
			{
				try {
					Thread.sleep(command.getDelay());
				} catch (InterruptedException e) {
					System.out.println("��!��ָ���ӳٱ��ж�");
				}
			}
			executive.addTask(command);
//			System.out.println("��debug�����ָ�������ִ��ģ��:"+command);
			
			if(index==commands.size()-1)
			{
				index=0;
				endTime=new Date().getTime();
				useTime=endTime-startTime;
				startTime=endTime;
				if(useTime<time)
				{
					try {
						System.out.println("��debug��ѭ�������ӳ�:"+(time-useTime));
						Thread.sleep(time-useTime);
					} catch (InterruptedException e) {
						System.out.println("��!��ѭ�������ӳٱ��ж�");
					}
				}
			}
			else 
				index++;
		}
	}

	public long getTime() {
		return time;
	}

	public void setTime(long time) {
		this.time = time;
	}

}
