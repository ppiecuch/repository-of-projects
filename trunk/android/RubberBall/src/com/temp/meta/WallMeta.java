package com.temp.meta;

import org.anddev.andengine.entity.shape.IShape;

public class WallMeta implements Meta {
	
	private IShape shape;
	
	public WallMeta(IShape shape) {
		this.shape=shape;
	}

	@Override
	public IShape getShape() {
		return this.shape;
	}

	@Override
	public int getType() {
		return TYPE_WALL;
	}

}
