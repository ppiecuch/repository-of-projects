package org.tinder.studio.warbird.enemy;

public class Command {
	
	public String type;
	public int delay;

	
	public Command(String type,int delay){
		this.type=type;
		this.delay=delay;
	}
	
	@Override
	public String toString() {
		return "type:"+type+",delay:"+delay;
	}
}
