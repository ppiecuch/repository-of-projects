package module;

import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.LinkedList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import core.TaskThread;

/**
 * ��ʱ��
 * @author Micheal
 * �߳�ִ�У���ϵͳʱ��ﵽ�����ӳ�ʱ��Ҫ��ʱ��������ָ���ִ����������
 *
 */
public class Calculagraph extends Thread implements Externalizable {
	
	private static final long serialVersionUID = 1L;

	private static TaskThread executive;
	
	private String title;
	private List<Operation> operations;
	private boolean runnable=true;
	private Timer timer;
	
	public Calculagraph(){}
	
	public Calculagraph(String title)
	{
		this.title=title;
	}

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
			op.writeExternal(out);
		}

	}
	
	@Override
	public synchronized void start() {
		super.start();
		executive=TaskThread.getInstance("TaskThread");
		executive.start();
	}
	
	@Override
	public void run() {
		Label:while(runnable)
		{
			if(timer==null)
			{
				timer=new Timer(false);		//������daemon�߳�
			}
			int count=1;
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
				if(runnable)
				{
					System.out.println("ִ�в���:"+count+++"/"+operations.size()+"-->"+op);
				}
				else
				{
					break Label;
				}
			}
		}
		timer.cancel();
		executive.close();
		executive=null;
		System.out.println("--��ʱ���ѹر�--");
	}
	
	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public synchronized void close()
	{
		this.runnable=false;
		this.notify();
	}

	public List<Operation> getOperations() {
		return operations;
	}

	public void setOperations(List<Operation> operations) {
		this.operations = operations;
	}
	
	public void addOperation(Operation operation)
	{
		if(this.operations==null)
		{
			this.operations=new LinkedList<Operation>();
		}
		this.operations.add(operation);
	}

}