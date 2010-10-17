package org.tinder.studio.warbird.enemy;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.tinder.studio.warbird.Plane;

import android.util.Log;

public class Commander extends Thread{
	
	private Map<String,Plane> map;
	private List<Command> commands;
	
	
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
	public void addCommand(String type,int delay){
		addCommand(new Command(type,delay));
	}
	
	@Override
	public void run() {
		for(Command c:commands){
			try {
				Thread.sleep(c.delay);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			Plane plane=map.get(c.type).clone();
			Log.d("Commander","execute command:"+c+",add:"+plane);
			synchronized (Plane.LOCK_ENEMY) {
				Plane.getEnemies().add(plane);
			}
		}
	}

}
