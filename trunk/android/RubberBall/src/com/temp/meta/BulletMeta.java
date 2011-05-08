package com.temp.meta;

import org.anddev.andengine.entity.shape.IShape;

public class BulletMeta implements Meta {
	
	private IShape shape;
	
	public BulletMeta(IShape shape) {
		this.shape=shape;
	}

	@Override
	public int getType() {
		return TYPE_BULLET;
	}
	
	@Override
	public IShape getShape() {
		return this.shape;
	}

}
