package org.tinder.studio.warbird.enemy;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.equip.Equip;

import android.util.Log;

public class Commander extends Thread{
	
	private Map<String,Plane> map;
	private List<Command> commands;
	private boolean runnable=true;
	private static Commander instance;
	private Commander(){}
	
	
	/*×¢²á»úÐÍ*/
	public void register(String type,Plane plane){
		if(map==null)
			map=new HashMap<String, Plane>();
		map.put(type,plane);
	}
	
	/*Ìí¼ÓÃüÁî*/
	public void addCommand(Command command){
		if(commands==null)
			commands=new LinkedList<Command>();
		commands.add(command);
	}
	public Command addCommand(String type,int delay,boolean luck){
		Command command=new Command(type,delay,luck);
		addCommand(command);
		return command;
	}
	
	@Override
	public void run() {
		for(Command c:commands){
			if(runnable)
			{
				if(c.delay>0)
				{
					try {
						Thread.sleep(c.delay);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
				Plane plane=map.get(c.type).clone();
				if(c.getAwards()!=null)
				{
					for(Equip e:c.getAwards())
						plane.addAward(e.clone());
				}
				Log.d("Commander","execute command:"+c+",add:"+plane);
				synchronized (Plane.LOCK_ENEMY) {
					Plane.getEnemies().add(plane);
				}
			}
		}
		Log.d("Commander","Stop");
	}
	
	
	
	public static Commander getInstance() {
		if(instance==null)
			instance=new Commander();
		return instance;
	}


	public void destroy(){
		this.runnable=false;
		this.interrupt();
	}

}
