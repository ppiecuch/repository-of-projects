package com.temp.meta;

import org.anddev.andengine.entity.shape.IShape;

public interface Meta {
	
	public static final int TYPE_BULLET=1;
	public static final int TYPE_BALL=2;
	public static final int TYPE_WALL=3;
	
	public int getType();
	public IShape getShape();

}
