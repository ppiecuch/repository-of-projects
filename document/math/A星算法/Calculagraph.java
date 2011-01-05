

import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

/**
 * 计划器，多例模式
 * @author Micheal Hong
 *
 */
public class Calculagraph {
	
	private List<String> titles=new LinkedList<String>();//titles[0]指markTimes[1]-markTimes[0]的标题，以此类推
	private List<Long> markTimes=new LinkedList<Long>();
	private String id=null;
	
	private static HashMap<String,Calculagraph> instances=new HashMap<String, Calculagraph>();
	
	private Calculagraph(String id){
		this.id=id;
	}
	
	public void mark(String title)
	{
		if(markTimes.size()>0)titles.add(title);
		markTimes.add(new Date().getTime());
	}
	
	public void clear()
	{
		this.titles.clear();
		this.markTimes.clear();
	}
	
	/**
	 * 从哈希表删除对象引用
	 * @param id
	 * @return
	 */
	public static Calculagraph remove(String id)
	{
		Calculagraph calculagraph=null;
		if(instances.containsKey(id))
		{
			calculagraph=instances.remove(id);
		}
		return calculagraph;
	}
	
	/**
	 * 获取与id对应的对象，如果不存在则创建之
	 * @param id
	 * @return
	 */
	public static Calculagraph getInstance(String id)
	{
		if(instances.containsKey(id)==false)
		{
			instances.put(id,new Calculagraph(id));
		}
		return instances.get(id);
	}
	
	public String getId() {
		return id;
	}
	
	public String getInfo()
	{
		StringBuffer buffer=new StringBuffer();
		for(int i=0;i<titles.size();i++)
		{
			if(buffer.length()>0)buffer.append(';');
			buffer.append(titles.get(i));
			buffer.append(':');
			buffer.append(markTimes.get(i+1)-markTimes.get(i));
			buffer.append("ms");
		}
		buffer.append(';');
		buffer.append("totalTime:");
		buffer.append(markTimes.get(markTimes.size()-1)-markTimes.get(0));
		buffer.append("ms");
		return "{id:"+id+","+buffer.toString()+"}";
	}
	
	public static void main(String[] args) throws InterruptedException
	{
		Calculagraph.getInstance("t").mark(null);
		Thread.sleep(350);
		Calculagraph.getInstance("t").mark("1");
		Thread.sleep(300);
		Calculagraph.getInstance("t").mark("2");
		System.out.println(Calculagraph.getInstance("t").getInfo());
	}

}
