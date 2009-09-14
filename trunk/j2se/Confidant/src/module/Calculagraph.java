package module;

import java.io.Externalizable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.util.LinkedList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.sun.org.apache.bcel.internal.generic.IINC;

import core.TaskThread;

/**
 * 计时器
 * @author Micheal
 * 线程执行，当系统时间达到操作延迟时间要求时，将操作指令传入执行器队列中
 *
 */
public class Calculagraph extends Thread implements Externalizable {
	
	private static final TaskThread executive=TaskThread.getInstance("Executive");
	
	private String title;
	private List<Operation> operations;
	private boolean runnable=true;
	private Timer timer;

	public void readExternal(ObjectInput in) throws IOException,
			ClassNotFoundException {
		this.title=in.readUTF();
		int count=in.readInt();
		in.readChar();
		operations=new LinkedList<Operation>();
		for(int i=0;i<count;i++)
		{
			Operation operation=new Operation();
			operation.readExternal(in);
			
			operations.add(operation);
		}

	}

	public void writeExternal(ObjectOutput out) throws IOException {
		out.writeUTF(title);
		out.writeInt(operations.size());
		out.writeChar('\n');
		for(Operation op:operations)
		{
			System.out.println(op);
			op.writeExternal(out);
		}

	}
	
	@Override
	public void start() {
		
//		Recorder.start();
//		try {
//			Thread.sleep(10000);
//		} catch (InterruptedException e) {
//			e.printStackTrace();
//		}
//		Recorder.stop();
//		this.operations=Recorder.getOperations();
//
//		File file =new File("D:/file.txt");
//		try {
//			file.createNewFile();
//			FileOutputStream fos=new FileOutputStream(file);
//			ObjectOutputStream oos=new ObjectOutputStream(fos);
//			writeExternal(oos);
//			oos.close();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
		
		File file =new File("D:/file.txt");
		try {
			FileInputStream fis=new FileInputStream(file);
			ObjectInputStream ois=new ObjectInputStream(fis);
			readExternal(ois);
			ois.close();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		super.start();
	}
	
	@Override
	public void run() {
		while(runnable)
		{
			if(timer==null)
			{
				timer=new Timer(false);		//创建非daemon线程
			}
			for(Operation op:operations)
			{
				final InstructionTask task=new InstructionTask();
				switch(op.getMode())
				{
				case Operation.MODE_KEYBOARD:
					task.setInstruction(op.getInstruction());
					break;
				case Operation.MODE_MOUSE:
					task.setPoint(op.getPoint());
					break;
				default:
					continue;
				}
				timer.schedule(new TimerTask(){
					@Override
					public void run() {
						executive.addTask(task);
						synchronized(Calculagraph.this){
							Calculagraph.this.notify();
						}
					}},op.getDelay());
				try {
					synchronized(this)
					{
						this.wait();
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
//			/***/return;
		}
		timer.cancel();
		executive.close();
	}
	
	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public void close()
	{
		this.runnable=false;
	}

	public static void main(String[] args)
	{
		Calculagraph calculagraph=new Calculagraph();
		calculagraph.setTitle("第一次");
		calculagraph.start();
		executive.start();
	}

}
