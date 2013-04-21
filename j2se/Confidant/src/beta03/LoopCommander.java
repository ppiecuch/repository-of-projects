package beta03;

import java.util.Date;

public class LoopCommander extends Commander implements Runnable{
	
	private Thread thread;
	private boolean runnable=true;
	private long time;		//时间间隔
	
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
					System.out.println("【!】指令延迟被中断");
				}
			}
			executive.addTask(command);
//			System.out.println("【debug】添加指令给命令执行模块:"+command);
			
			if(index==commands.size()-1)
			{
				index=0;
				endTime=new Date().getTime();
				useTime=endTime-startTime;
				startTime=endTime;
				if(useTime<time)
				{
					try {
						System.out.println("【debug】循环块间隔延迟:"+(time-useTime));
						Thread.sleep(time-useTime);
					} catch (InterruptedException e) {
						System.out.println("【!】循环块间隔延迟被中断");
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
