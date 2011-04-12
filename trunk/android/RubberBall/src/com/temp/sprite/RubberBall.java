package com.temp.sprite;

import org.anddev.andengine.engine.handler.physics.PhysicsHandler;
import org.anddev.andengine.entity.sprite.AnimatedSprite;
import org.anddev.andengine.opengl.texture.region.TiledTextureRegion;

import com.temp.MainMenu;

public class RubberBall extends AnimatedSprite {
	
	public static final float DEMO_VELOCITY = 100.0f;
	
	private final PhysicsHandler mPhysicsHandler;

	public RubberBall(final float pX, final float pY, final TiledTextureRegion pTextureRegion) {
		super(pX, pY, pTextureRegion);
		this.mPhysicsHandler = new PhysicsHandler(this);
		this.registerUpdateHandler(this.mPhysicsHandler);
	}

	@Override
	protected void onManagedUpdate(final float pSecondsElapsed) {
		if(this.mX < 0) {
			this.mPhysicsHandler.setVelocityX(DEMO_VELOCITY);
		} else if(this.mX + this.getHeight() > MainMenu.CAMERA_HEIGHT) {
			this.mPhysicsHandler.setVelocityX(-DEMO_VELOCITY);
		}

		if(this.mY < 0) {
			this.mPhysicsHandler.setVelocityY(DEMO_VELOCITY);
		} else if(this.mY + this.getWidth() > MainMenu.CAMERA_WIDTH) {
			this.mPhysicsHandler.setVelocityY(-DEMO_VELOCITY);
		}

		super.onManagedUpdate(pSecondsElapsed);
	}

}
