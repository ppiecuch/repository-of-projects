package com.temp.meta;

import org.anddev.andengine.entity.shape.IShape;
import org.anddev.andengine.extension.physics.box2d.PhysicsConnector;

public class BallMeta implements Meta {
	
	private IShape shape;
	private PhysicsConnector physicsConnector;
	
	public BallMeta(IShape shape,PhysicsConnector physicsConnector){
		this.shape=shape;
		this.physicsConnector=physicsConnector;
	}

	@Override
	public int getType() {
		return TYPE_BALL;
	}

	@Override
	public IShape getShape() {
		return this.shape;
	}

	public PhysicsConnector getPhysicsConnector() {
		return physicsConnector;
	}

}
