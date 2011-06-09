package com.temp.sprite;

import javax.microedition.khronos.opengles.GL10;

import org.anddev.andengine.engine.handler.physics.PhysicsHandler;
import org.anddev.andengine.entity.IEntity;
import org.anddev.andengine.entity.particle.ParticleSystem;
import org.anddev.andengine.entity.particle.emitter.CircleOutlineParticleEmitter;
import org.anddev.andengine.entity.particle.initializer.AlphaInitializer;
import org.anddev.andengine.entity.particle.initializer.ColorInitializer;
import org.anddev.andengine.entity.particle.initializer.VelocityInitializer;
import org.anddev.andengine.entity.particle.modifier.AlphaModifier;
import org.anddev.andengine.entity.particle.modifier.ColorModifier;
import org.anddev.andengine.entity.particle.modifier.ExpireModifier;
import org.anddev.andengine.entity.particle.modifier.ScaleModifier;
import org.anddev.andengine.entity.sprite.AnimatedSprite;
import org.anddev.andengine.entity.sprite.Sprite;
import org.anddev.andengine.extension.physics.box2d.PhysicsFactory;
import org.anddev.andengine.extension.physics.box2d.PhysicsWorld;
import org.anddev.andengine.extension.physics.box2d.util.Vector2Pool;
import org.anddev.andengine.opengl.texture.region.BaseTextureRegion;
import org.anddev.andengine.opengl.texture.region.TextureRegion;
import org.anddev.andengine.opengl.texture.region.TiledTextureRegion;

import android.util.Log;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.Body;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.BodyDef.BodyType;
import com.temp.MainMenu;
import com.temp.meta.BulletMeta;

public class BulletBall extends AnimatedSprite {
	
	private PhysicsHandler physicsHandler;
	private final CircleOutlineParticleEmitter particleEmitter;
	private final ParticleSystem particleSystem;
	private final Body body;
	private VelocityInitializer initializer;
	private static FixtureDef FIXTURE_DEF = PhysicsFactory.createFixtureDef(1, 1.03f, 0.6f);
	private float velocityX,velocityY,lastVelocityX,lastVelocityY;
	private Sprite highLight;
	private static final int VELOCITY_LENGTH=10;
	private boolean needReset=false;
	private boolean needRecord=false;
	private int status=0;

	public BulletBall(float pX, float pY,TiledTextureRegion pTextureRegion,TextureRegion hTextureRegion,TextureRegion mParticleTextureRegion,PhysicsWorld physicsWorld) {
		super(pX, pY,pTextureRegion);
		this.highLight=new Sprite(pX, pY, hTextureRegion);
//		this.physicsHandler = new PhysicsHandler(this){
//			@Override
//			protected void onUpdate(float pSecondsElapsed, IEntity pEntity) {
//				setVelocity(10000);
//				Log.v("handler",String.valueOf(this.mVelocityX));
//				super.onUpdate(pSecondsElapsed, pEntity);
//			}
//		};
//		physicsHandler = new PhysicsHandler(this){
//			@Override
//			protected void onUpdate(float pSecondsElapsed, IEntity pEntity) {
//				Log.d("Bullet",getVelocityX()+","+getVelocityY());
//			}
//		};
//		this.registerUpdateHandler(physicsHandler);
		this.body = PhysicsFactory.createCircleBody(physicsWorld, this, BodyType.DynamicBody, FIXTURE_DEF);
		this.body.setUserData(new BulletMeta(this));
		this.particleEmitter= new CircleOutlineParticleEmitter(MainMenu.CAMERA_WIDTH * 0.5f, MainMenu.CAMERA_HEIGHT * 0.5f + 20, 10);
		this.particleSystem = new ParticleSystem(particleEmitter, 40, 40, 60, mParticleTextureRegion);
		this.initializer=new VelocityInitializer(0);
		//定义粒子效果
		particleSystem.addParticleInitializer(new ColorInitializer(1, 0, 0));
		particleSystem.addParticleInitializer(new AlphaInitializer(1));
		particleSystem.addParticleInitializer(initializer);
		particleSystem.setBlendFunction(GL10.GL_SRC_ALPHA, GL10.GL_ONE);
		particleSystem.addParticleModifier(new ExpireModifier(0, 0.4f));
		particleSystem.addParticleModifier(new ScaleModifier(2.0f, 1.0f, 0, 0.8f));
//		particleSystem.addParticleInitializer(new RotationInitializer(0.0f, 90.0f));
		particleSystem.addParticleModifier(new ColorModifier(0.5f, 1, 0, 0.5f, 1, 1, 0, 1));
		particleSystem.addParticleModifier(new ColorModifier(1, 1, 0.5f, 1, 0, 1, 1, 2));
		particleSystem.addParticleModifier(new AlphaModifier(0, 1, 0, 0.5f));
		particleSystem.addParticleModifier(new AlphaModifier(1, 0, 0.5f, 2));
	}
	
	@Override
	protected void onManagedUpdate(final float pSecondsElapsed) {
//		velocityX=-this.velocityX;
//		velocityY=-this.velocityY;
		particleEmitter.setCenter(this.getX(),this.getY());
		this.highLight.setPosition(this.getX(),this.getY());
//		float halfVelocityX=velocityX/2;
//		float halfVelocityY=velocityY/2;
//		this.initializer.setVelocity(halfVelocityX, velocityX, halfVelocityY, velocityY);
		
//		Vector2 velocity = Vector2Pool.obtain(30, 30); 
//
//		this.body.setLinearVelocity(velocity);
//		Vector2Pool.recycle(velocity);
		this.velocityX=this.body.getLinearVelocity().x;
		this.velocityY=this.body.getLinearVelocity().y;
		switch(status)
		{
		case 0:
			break;
		case 1:
			this.lastVelocityX=this.body.getLinearVelocity().x;
			this.lastVelocityY=this.body.getLinearVelocity().y;
			status=0;
			break;
		case 2:
			//如果是左右弹,记录Y方向速度分量,防止此分量衰减
			if(velocityX<0&&lastVelocityX>0||velocityX>0&&lastVelocityX<0)
			{
				this.velocityY=this.lastVelocityY;
			}
		case 3:
			float len=(float) Math.sqrt(velocityX*velocityX+velocityY*velocityY);
			this.velocityX=this.velocityX*VELOCITY_LENGTH/len;
			this.velocityY=this.velocityY*VELOCITY_LENGTH/len;
			Vector2 velocity = Vector2Pool.obtain(velocityX,velocityY); 
			this.body.setLinearVelocity(velocity);
			Vector2Pool.recycle(velocity);
			status=0;
		}
		if(needRecord)
		{
			
		}
		
		if(needReset)
		{
			
			needReset=false;
		}
		
//		Log.d("Bullet",velocityX+","+velocityY);
		
		super.onManagedUpdate(pSecondsElapsed);
		
	}
	
	

	public ParticleSystem getParticleSystem() {
		return particleSystem;
	}
	
//	public void setLinearVelocity(float x,float y){
//		this.body.setActive(false);
//		this.body.setLinearVelocity(new Vector2(x, y));
//		this.body.setActive(true);
//	}
//	
//	public void applyForce(int x,int y){
//		this.body.applyForce(new Vector2(x,y),body.getLocalCenter());
//	}

	public Body getBody() {
		return body;
	}
	
	public float getVelocityX(){
		return velocityX;
	}
	
	public float getVelocityY(){
		return velocityY;
	}
	
	public void record()
	{
		needRecord=true;
	}
	
	public void resetSpeed(){
//		synchronized (speedLock) {
			needReset=true;
//		}
	}

	
	public void setStatus(int status){
		this.status=status;
	}
	public Sprite getHighLight(){
		return highLight;
	}
	

}
