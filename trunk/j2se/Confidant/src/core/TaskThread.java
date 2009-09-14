package core;

import java.util.HashMap;
import java.util.Vector;

/**
 * Task Processing Thread.It is enable to add Task and perform task
 * synchronously. All tasks is restored in the task queue. If there is task(s)
 * in the task queue,it(them) will be performed by the thread automaticlly. Or
 * the thread will be turned into waiting for waking. <br>
 * Mode:Multiton Mode <br>
 * �������߳�,ʵ��ͬ����Ӽ�ִ������,���񱣴����������֮��. ����������д�������ʱ,�߳̽��Զ�������д���;�����߳̽���ȴ�״̬. <br>
 * ģʽ:����ģʽ
 * 
 * @author Micheal
 * @see Task
 */
public final class TaskThread extends Thread {

	private static HashMap<String, TaskThread> instanceMap = new HashMap<String, TaskThread>();

	private Vector<Task> taskQueue = new Vector<Task>();

	private boolean runnable = true;

	/**
	 * Return a task thread instance corresponding to threadId, if it don't
	 * exist,create one and return it. Note that each <code>threadId</code> is
	 * unique to the others. <br>
	 * ����һ����threadId��Ӧ���������߳�ʵ��,���������������ʵ��, ����д���������һ����ʵ��,��ʵ����threadId���Ӧ
	 * 
	 * @param threadId
	 * @return
	 */
	public static TaskThread getInstance(String threadId) {
		if (instanceMap.containsKey(threadId) == false) {
			instanceMap.put(threadId, new TaskThread());
		}
		return instanceMap.get(threadId);
	}

	private TaskThread() {
	}

	@Override
	public void run() {
		Task task;
		while (runnable) {
			synchronized (taskQueue) { // apply for the task queue's obj lock
				while (taskQueue.isEmpty()) {
					taskQueue.notify(); // release the task queue's obj lock
					try {
						taskQueue.wait(); // turn into waiting for being waken
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
				/* when the task queue is not empty */
				task = taskQueue.remove(0); // fetch the head task
				taskQueue.notify(); // release the task queue's obj lock
			}
			task.perform(); // perform the task
		}
		System.out.println("TaskThread close");
	}

	/**
	 * Add task into task queue synchronously and then wake the task thread.
	 * <br>
	 * ���������������񲢻����̶߳Զ��н��д���
	 * 
	 * @param task
	 */
	public void addTask(Task task) {
		synchronized (taskQueue) { // apply for the task queue's obj lock
			taskQueue.add(task);
			taskQueue.notify(); // release the task queue's obj lock
		}
	}
	
	/**
	 * Close task thread
	 * <br>
	 * �ر��������߳�
	 *
	 */
	public void close(){
		this.runnable=false;
	}

}
