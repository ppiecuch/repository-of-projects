package org.tinder.studio.warbird.enemy;

import java.util.LinkedList;
import java.util.List;

import org.tinder.studio.warbird.Util;
import org.tinder.studio.warbird.equip.Equip;

public class Command {
	
	public String type;
	public int delay;
	private List<Equip> awards;

	
	public Command(String type,int delay,boolean luck){
		this.type=type;
		this.delay=delay;
		if(luck)
		{
			if(Util.random(1,10)==1){
				awards.add(Equip.generateRandomEquip());
			}
		}
	}
	
	public Command addAward(Equip equip){
		if(this.awards==null)
			this.awards=new LinkedList<Equip>();
		this.awards.add(equip);
		return this;
	}
	
	
	
	public List<Equip> getAwards() {
		return awards;
	}

	@Override
	public String toString() {
		return "type:"+type+",delay:"+delay;
	}
}
