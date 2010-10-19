package org.tinder.studio.warbird;

import java.util.LinkedList;

import android.graphics.Bitmap;

public class Feature {
	
	public static final int CAMP_WHITE=1;
	public static final int CAMP_BLACK=2;
	
	public static final Feature FEATURE_1=new Feature(CAMP_WHITE, 10);
	public static final Feature FEATURE_2=new Feature(CAMP_WHITE, 20);
	public static final Feature FEATURE_E1=new Feature(CAMP_BLACK, 10);
	
	
	private int camp;					//’Û”™
	private int execution;				//…±…À¡¶
	private LinkedList<Bitmap> frames;	//÷°
	
	private Feature(int camp,int execution){
		this.camp=camp;
		this.execution=execution;
	}
	
	public void addFrame(Bitmap frame){
		if(this.frames==null)
			this.frames=new LinkedList<Bitmap>();
		this.frames.add(frame);
	}

	public int getCamp() {
		return camp;
	}

	public void setCamp(int camp) {
		this.camp = camp;
	}

	public int getExecution() {
		return execution;
	}

	public void setExecution(int execution) {
		this.execution = execution;
	}

	public LinkedList<Bitmap> getFrames() {
		return frames;
	}
	
	

}
