package com.temp.sprite;

import java.util.ArrayList;
import java.util.Random;

import javax.microedition.khronos.opengles.GL10;

import org.anddev.andengine.engine.camera.Camera;
import org.anddev.andengine.engine.handler.physics.PhysicsHandler;
import org.anddev.andengine.entity.modifier.ScaleModifier;
import org.anddev.andengine.entity.sprite.AnimatedSprite;
import org.anddev.andengine.entity.sprite.Sprite;
import org.anddev.andengine.extension.physics.box2d.util.constants.PhysicsConstants;
import org.anddev.andengine.input.touch.TouchEvent;
import org.anddev.andengine.opengl.texture.region.TextureRegion;
import org.anddev.andengine.opengl.texture.region.TiledTextureRegion;

import android.app.SearchManager;
import android.hardware.SensorManager;
import android.util.Log;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.Body;
import com.badlogic.gdx.physics.box2d.Fixture;
import com.temp.MainMenu;
import com.temp.meta.BallMeta;

public class RubberBall extends Sprite {
	
	
	private static final int VELOCITY_DEFAULT=50;
//	private final PhysicsHandler mPhysicsHandler;
	private Body body;
	private boolean mature;
	private float scale=1;
//	private static final Random RANDOM=new Random();

	public RubberBall(final float pX, final float pY, final TextureRegion pTextureRegion) {
		super(pX, pY, pTextureRegion);
//		this.mPhysicsHandler = new PhysicsHandler(this);
		this.setScaleCenter(0,0);
//		this.registerUpdateHandler(this.mPhysicsHandler);
		this.mature=false;
	}
	
	public void mature(){
		this.mature=true;
	}
	
//	@Override
//	public boolean onAreaTouched(TouchEvent pSceneTouchEvent,
//			float pTouchAreaLocalX, float pTouchAreaLocalY) {
//		scale+=0.1f;
//		this.setScale(scale);
//		return true;
//	}
	
//	private Vector2 randomVector2(){
//		return new Vector2(RANDOM.nextFloat(),RANDOM.nextFloat());
//	}
	

	@Override
	protected void onManagedUpdate(final float pSecondsElapsed) {
		if(mature){
//			float velocityX=this.mPhysicsHandler.getVelocityX();
//			float velocityY=this.mPhysicsHandler.getVelocityX();
//			Log.v("rubber",String.valueOf(velocityX));
//			if(this.mX < 0) {
//				this.mPhysicsHandler.setVelocityX(velocityX);
//			} else if(this.mX + this.getHeight() > MainMenu.POOL_WIDTH) {
//				this.mPhysicsHandler.setVelocityX(-velocityX);
//			}
//
//			if(this.mY < 0) {
//				this.mPhysicsHandler.setVelocityY(velocityY);
//			} else if(this.mY + this.getWidth() > MainMenu.POOL_HEIGHT) {
//				this.mPhysicsHandler.setVelocityY(-velocityY);
//			}
			body.applyForce(new Vector2(0, SensorManager.GRAVITY_EARTH),body.getPosition());
		}else{
//			this.registerEntityModifier(new ScaleModifier(1, 1,1.5f));
			scale = scale + pSecondsElapsed*3;
	        setScaleCenter(getWidth()/2, getHeight()/2);
	        setScale(scale);
			 ArrayList<Fixture> fixtureList = body.getFixtureList();         
		     fixtureList.get(0).getShape().setRadius((getWidthScaled()/2) / PhysicsConstants.PIXEL_TO_METER_RATIO_DEFAULT);
		                        
		     // move bubble's body to touch position
//		     body.setTransform(new Vector2((float)(mTouchX)/PhysicsConstants.PIXEL_TO_METER_RATIO_DEFAULT, 
//		                                                        (float)(mTouchY)/PhysicsConstants.PIXEL_TO_METER_RATIO_DEFAULT), 0);

		}
		super.onManagedUpdate(pSecondsElapsed);
	}

	public boolean isMature() {
		return mature;
	}

	public void setBody(Body body) {
		this.body = body;
	}

	public Body getBody() {
		return body;
	}
	
	public BallMeta getMeta(){
		return (BallMeta) this.getBody().getUserData();
	}

	
}
