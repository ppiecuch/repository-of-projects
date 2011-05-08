package com.temp;

import javax.microedition.khronos.opengles.GL10;

import org.anddev.andengine.engine.Engine;
import org.anddev.andengine.engine.camera.Camera;
import org.anddev.andengine.engine.handler.physics.PhysicsHandler;
import org.anddev.andengine.engine.options.EngineOptions;
import org.anddev.andengine.engine.options.EngineOptions.ScreenOrientation;
import org.anddev.andengine.engine.options.resolutionpolicy.RatioResolutionPolicy;
import org.anddev.andengine.entity.particle.modifier.ScaleModifier;
import org.anddev.andengine.entity.particle.ParticleSystem;
import org.anddev.andengine.entity.particle.emitter.CircleOutlineParticleEmitter;
import org.anddev.andengine.entity.particle.initializer.AlphaInitializer;
import org.anddev.andengine.entity.particle.initializer.ColorInitializer;
import org.anddev.andengine.entity.particle.initializer.RotationInitializer;
import org.anddev.andengine.entity.particle.initializer.VelocityInitializer;
import org.anddev.andengine.entity.particle.modifier.AlphaModifier;
import org.anddev.andengine.entity.particle.modifier.ColorModifier;
import org.anddev.andengine.entity.particle.modifier.ExpireModifier;
import org.anddev.andengine.entity.primitive.Rectangle;
import org.anddev.andengine.entity.scene.Scene;
import org.anddev.andengine.entity.scene.Scene.IOnSceneTouchListener;
import org.anddev.andengine.entity.scene.background.ColorBackground;
import org.anddev.andengine.entity.shape.Shape;
import org.anddev.andengine.entity.sprite.AnimatedSprite;
import org.anddev.andengine.entity.sprite.Sprite;
import org.anddev.andengine.extension.physics.box2d.PhysicsConnector;
import org.anddev.andengine.extension.physics.box2d.PhysicsFactory;
import org.anddev.andengine.extension.physics.box2d.PhysicsWorld;
import org.anddev.andengine.extension.physics.box2d.util.Vector2Pool;
import org.anddev.andengine.input.touch.TouchEvent;
import org.anddev.andengine.opengl.texture.Texture;
import org.anddev.andengine.opengl.texture.TextureOptions;
import org.anddev.andengine.opengl.texture.region.TextureRegion;
import org.anddev.andengine.opengl.texture.region.TextureRegionFactory;
import org.anddev.andengine.opengl.texture.region.TiledTextureRegion;
import org.anddev.andengine.sensor.accelerometer.AccelerometerData;
import org.anddev.andengine.sensor.accelerometer.IAccelerometerListener;
import org.anddev.andengine.ui.activity.BaseGameActivity;

import android.hardware.SensorManager;
import android.util.Log;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.Body;
import com.badlogic.gdx.physics.box2d.Contact;
import com.badlogic.gdx.physics.box2d.ContactListener;
import com.badlogic.gdx.physics.box2d.Fixture;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.BodyDef.BodyType;
import com.temp.meta.BallMeta;
import com.temp.meta.BulletMeta;
import com.temp.meta.Meta;
import com.temp.meta.WallMeta;
import com.temp.sprite.BulletBall;
import com.temp.sprite.RubberBall;

public class Game extends BaseGameActivity implements IAccelerometerListener,IOnSceneTouchListener,ContactListener {
	
	private Camera mCamera;

	private Texture mTexture1;
	private Texture mTexture2;
//	private TiledTextureRegion mFaceTextureRegion;
//	private TiledTextureRegion mFaceTextureRegion2;
	private TextureRegion mFaceTextureRegion;
	private TextureRegion mFaceTextureRegion2;
	
	private Texture mTexture;
	private TextureRegion mParticleTextureRegion;
	
	private static final FixtureDef FIXTURE_DEF = PhysicsFactory.createFixtureDef(1, 0.5f, 0.5f);
//	private PhysicsWorld mPhysicsWorld1;
	private static final float GRAVITY=SensorManager.GRAVITY_EARTH*0.8f;
	private PhysicsWorld mPhysicsWorld2;
	private RubberBall temp;
	private Vector2 frictionVector=new Vector2(0,50);
	int y1=(MainMenu.CAMERA_HEIGHT-MainMenu.POOL_HEIGHT)/2;
	int y2=(MainMenu.CAMERA_HEIGHT+MainMenu.POOL_HEIGHT)/2;
	int x1=(MainMenu.CAMERA_WIDTH-MainMenu.POOL_WIDTH)/2;
	int x2=(MainMenu.CAMERA_WIDTH+MainMenu.POOL_WIDTH)/2;
	
	
	@Override
	public void onLoadComplete() {
		// TODO Auto-generated method stub

	}

	@Override
	public Engine onLoadEngine() {
		this.mCamera = new Camera(0, 0, MainMenu.CAMERA_WIDTH, MainMenu.CAMERA_HEIGHT);
//		return new Engine(new EngineOptions(true, ScreenOrientation.PORTRAIT, new RatioResolutionPolicy(MainMenu.CAMERA_HEIGHT, MainMenu.CAMERA_WIDTH), this.mCamera));
		final EngineOptions engineOptions = new EngineOptions(true, ScreenOrientation.PORTRAIT, new RatioResolutionPolicy(MainMenu.CAMERA_WIDTH, MainMenu.CAMERA_HEIGHT), this.mCamera);
		engineOptions.getTouchOptions().setRunOnUpdateThread(true);
		return new Engine(engineOptions);
	}

	/**
	 * 载入资源
	 */
	@Override
	public void onLoadResources() {
		this.mTexture1 = new Texture(32, 32, TextureOptions.BILINEAR_PREMULTIPLYALPHA);
		this.mTexture2 = new Texture(32, 32, TextureOptions.BILINEAR_PREMULTIPLYALPHA);
//		this.mFaceTextureRegion = TextureRegionFactory.createTiledFromAsset(this.mTexture, this, "gfx/face_circle_tiled.png", 0, 0, 1, 1);
//		this.mFaceTextureRegion2 = TextureRegionFactory.createTiledFromAsset(this.mTexture, this, "gfx/face_circle_tiled2.png", 0, 0, 1, 1);
		this.mFaceTextureRegion = TextureRegionFactory.createFromAsset(this.mTexture1, this, "gfx/face_circle_tiled.png",0,0);
		this.mFaceTextureRegion2 = TextureRegionFactory.createFromAsset(this.mTexture2, this, "gfx/face_circle_tiled2.png",0,0);
		this.mEngine.getTextureManager().loadTexture(this.mTexture1);
		this.mEngine.getTextureManager().loadTexture(this.mTexture2);
		
		this.mTexture = new Texture(32, 32, TextureOptions.BILINEAR_PREMULTIPLYALPHA);
		this.mParticleTextureRegion = TextureRegionFactory.createFromAsset(this.mTexture, this, "gfx/particle_point.png", 0, 0);
		this.mEngine.getTextureManager().loadTexture(this.mTexture);
		
//		this.enableAccelerometerSensor(this);
	}
	

	
	@Override
	public boolean onSceneTouchEvent(final Scene pScene, final TouchEvent pSceneTouchEvent) {
		if(this.mPhysicsWorld2 != null) {
			float x=pSceneTouchEvent.getX();
			float y=pSceneTouchEvent.getY();
			if(x>x1&&x<x2-20&&y>y1&&y<y2-20)
			{
				if(pSceneTouchEvent.isActionDown()) {
					temp=generateFace(x, y, 0);
					return true;
				}else if(pSceneTouchEvent.isActionUp()){
					if(temp!=null)
					{
						temp.mature();
						temp=null;
					}
					return true;
				}
			}
		}
		return false;
	}
	
	@Override
	public void onAccelerometerChanged(final AccelerometerData pAccelerometerData) {
		final Vector2 gravity = Vector2Pool.obtain(pAccelerometerData.getY(), pAccelerometerData.getX());
		this.mPhysicsWorld2.setGravity(gravity);
		Vector2Pool.recycle(gravity);
	}

	@Override
	public Scene onLoadScene() {
		final Scene scene = new Scene(1);
		scene.setBackground(new ColorBackground(0.09804f, 0.6274f, 0.8784f));
		scene.setOnSceneTouchListener(this);
		
		
		
		this.mPhysicsWorld2 = new PhysicsWorld(new Vector2(0, 0), false);
		this.mPhysicsWorld2.setContactListener(this);
		//定义墙体
		final Shape ground = new Rectangle(x1, y2 - 10, MainMenu.POOL_WIDTH, 10);
		final Shape roof = new Rectangle(x1+32, y1, MainMenu.POOL_WIDTH-60, 10);
		final Shape left = new Rectangle(x1, y1+30, 10, MainMenu.POOL_HEIGHT-30);
		final Shape right = new Rectangle(x2 - 10, y1+30, 10, MainMenu.POOL_HEIGHT-30);
		final Shape ne=new Rectangle(x2-38,y1+13,42,10);
		final Shape we=new Rectangle(x1-2,y1+13,42,10);
		ne.setRotation(45);
		we.setRotation(-45);
		
		final FixtureDef wallFixtureDef1 = PhysicsFactory.createFixtureDef(0, 0.5f, 0.5f);
		Body groundBody=PhysicsFactory.createBoxBody(this.mPhysicsWorld2, ground, BodyType.StaticBody, wallFixtureDef1);
		Body roofBody=PhysicsFactory.createBoxBody(this.mPhysicsWorld2, roof, BodyType.StaticBody, wallFixtureDef1);
		Body leftBody=PhysicsFactory.createBoxBody(this.mPhysicsWorld2, left, BodyType.StaticBody, wallFixtureDef1);
		Body rightBody=PhysicsFactory.createBoxBody(this.mPhysicsWorld2, right, BodyType.StaticBody, wallFixtureDef1);
		Body neBody=PhysicsFactory.createBoxBody(this.mPhysicsWorld2, ne, BodyType.StaticBody, wallFixtureDef1);
		Body weBody=PhysicsFactory.createBoxBody(this.mPhysicsWorld2, we, BodyType.StaticBody, wallFixtureDef1);
		
		
		
		
		
		//设置用户数据
		groundBody.setUserData(new WallMeta(ground));
		roofBody.setUserData(new WallMeta(roof));
		leftBody.setUserData(new WallMeta(left));
		rightBody.setUserData(new WallMeta(right));
		neBody.setUserData(new WallMeta(ne));
		weBody.setUserData(new WallMeta(we));
		
		scene.getFirstChild().attachChild(ground);
		scene.getFirstChild().attachChild(roof);
		scene.getFirstChild().attachChild(left);
		scene.getFirstChild().attachChild(right);
		scene.getFirstChild().attachChild(ne);
		scene.getFirstChild().attachChild(we);

		scene.registerUpdateHandler(this.mPhysicsWorld2);
		

		
		

		

		//定义子弹球
		BulletBall face1;
		face1=new BulletBall(MainMenu.CAMERA_WIDTH/2, MainMenu.CAMERA_HEIGHT/2, this.mFaceTextureRegion2, this.mParticleTextureRegion,this.mPhysicsWorld2);
		scene.getLastChild().attachChild(face1.getParticleSystem());
//		face1.setLinearVelocity(1000, 100);
//		face1.applyForce(-2000,1000);
		scene.getLastChild().attachChild(face1);
		this.mPhysicsWorld2.registerPhysicsConnector(new PhysicsConnector(face1, face1.getBody(), true, false));
		
		face1=new BulletBall(MainMenu.CAMERA_WIDTH/2-20, MainMenu.CAMERA_HEIGHT/2, this.mFaceTextureRegion2, this.mParticleTextureRegion,this.mPhysicsWorld2);
		scene.getLastChild().attachChild(face1.getParticleSystem());
//		face1.setLinearVelocity(-100, 100);
//		face1.applyForce(200,100);
		scene.getLastChild().attachChild(face1);
		this.mPhysicsWorld2.registerPhysicsConnector(new PhysicsConnector(face1, face1.getBody(), true, false));
		
		
		return scene;
	}
	
	/**
	 * 创建橡皮球
	 * @param x
	 * @param y
	 * @param time
	 * @return
	 */
	private RubberBall generateFace(float x,float y, long time){
		final Scene scene = this.mEngine.getScene();
		final Body body2;
		RubberBall face2=new RubberBall(x, y,mFaceTextureRegion);
		body2 = PhysicsFactory.createCircleBody(this.mPhysicsWorld2, face2, BodyType.DynamicBody, FIXTURE_DEF);
		
		face2.setBody(body2);
		scene.getLastChild().attachChild(face2);
		PhysicsConnector physicsConnector=new PhysicsConnector(face2, body2, true, false);
		this.mPhysicsWorld2.registerPhysicsConnector(physicsConnector);
		
		body2.setUserData(new BallMeta(face2,physicsConnector));
		return face2;
	}
	
	private void doBallHitBullet(final RubberBall ball){
		final Scene scene = this.mEngine.getScene();
		if(ball.isMature()==false)
		{
//			final PhysicsConnector facePhysicsCaonnector = this.mPhysicsWorld2.getPhysicsConnectorManager().findPhysicsConnectorByShape(ball);
//			this.mPhysicsWorld2.unregisterPhysicsConnector(ball.getMeta().getPhysicsConnector());
//			this.mPhysicsWorld2.destroyBody(ball.getMeta().getPhysicsConnector().getBody());
//			scene.unregisterTouchArea(ball);
//			scene.getLastChild().detachChild(ball);
			this.runOnUpdateThread(new Runnable() {
				@Override
				public void run() {
					mPhysicsWorld2.unregisterPhysicsConnector(ball.getMeta().getPhysicsConnector());
					mPhysicsWorld2.destroyBody(ball.getMeta().getPhysicsConnector().getBody());
					scene.getLastChild().detachChild(ball);
				}
			});
		}
		/* Removing entities can only be done safely on the UpdateThread.
		 * Doing it while updating/drawing can
		 * cause an exception with a suddenly missing entity.
		 * Alternatively, there is a possibility to run the TouchEvents on the UpdateThread by default, by doing:
		 * engineOptions.getTouchOptions().setRunOnUpdateThread(true);
		 * when creating the Engine in onLoadEngine(); */
//		this.runOnUpdateThread(new Runnable() {
//			@Override
//			public void run() {
//				scene.detachChild(ball);
//			}
//		});
	}
	
	private void doBallHitWall(final RubberBall ball){
		ball.mature();
	}
	
	private void doBallHitBall(final RubberBall ball1,final RubberBall ball2){
//		ball1.mature();
		
		final Scene scene = this.mEngine.getScene();
		if(ball1.isMature()==false)
		{
			this.runOnUpdateThread(new Runnable() {
				@Override
				public void run() {
					mPhysicsWorld2.unregisterPhysicsConnector(ball1.getMeta().getPhysicsConnector());
					mPhysicsWorld2.destroyBody(ball1.getMeta().getPhysicsConnector().getBody());
					scene.getLastChild().detachChild(ball1);
				}
			});
		}else if(ball2.isMature()==false)
		{
			this.runOnUpdateThread(new Runnable() {
				@Override
				public void run() {
					mPhysicsWorld2.unregisterPhysicsConnector(ball2.getMeta().getPhysicsConnector());
					mPhysicsWorld2.destroyBody(ball2.getMeta().getPhysicsConnector().getBody());
					scene.getLastChild().detachChild(ball2);
				}
			});
		}
	}

	/**
	 * 碰撞前
	 */
	@Override
	public void beginContact(Contact contact) {
		Fixture a=contact.getFixtureA();
		Fixture b=contact.getFixtureB();
		Meta metaA=(Meta) a.getBody().getUserData();
		Meta metaB=(Meta) b.getBody().getUserData();
//		Meta meta=null;
		RubberBall rubberBall=null;
//		Vector2 vector=contact.GetWorldManifold().getPoints()[0];
		if(metaA.getType()==Meta.TYPE_BALL&&metaB.getType()==Meta.TYPE_BULLET)
		{
			rubberBall=(RubberBall)metaA.getShape();
			doBallHitBullet(rubberBall);
			contact.setEnabled(false);
//			meta=metaB;                        
		}
		else if(metaA.getType()==Meta.TYPE_BULLET&&metaB.getType()==Meta.TYPE_BALL)
		{
			rubberBall=(RubberBall)metaB.getShape();
			doBallHitBullet(rubberBall);
			contact.setEnabled(false);
//			meta=metaA;
		}
		else if(metaA.getType()==Meta.TYPE_BALL&&metaB.getType()==Meta.TYPE_WALL)
		{
			rubberBall=(RubberBall)metaA.getShape();
			doBallHitWall(rubberBall);
		}
		else if(metaA.getType()==Meta.TYPE_WALL&&metaB.getType()==Meta.TYPE_BALL)
		{
			rubberBall=(RubberBall)metaB.getShape();
			doBallHitWall(rubberBall);
		}
		else if(metaA.getType()==Meta.TYPE_BALL&&metaB.getType()==Meta.TYPE_BALL)
		{
//			rubberBall=(RubberBall)metaA.getShape();
//			if(rubberBall.isMature()==false){
//				doBallHitBall(rubberBall);
//			}
//			rubberBall=(RubberBall)metaB.getShape();
//			if(rubberBall.isMature()==false){
//				doBallHitBall(rubberBall);
//			}
			doBallHitBall((RubberBall)metaA.getShape(),(RubberBall)metaB.getShape());
		}

//		if(meta!=null)
//		{
//			BulletBall ball=((BulletBall)meta.getShape());
//			ball.getBody().applyForce(new Vector2((ball.getX()-vector.x)*0.05f,(ball.getY()-vector.y)*0.05f),new Vector2(ball.getX(),ball.getY()));
//			Log.v("bullet","force");
//		}
//		else{
//			if(metaA.getType()==Meta.TYPE_WALL&&metaB.getType()==Meta.TYPE_BULLET)
//				meta=metaB;
//			else if(metaB.getType()==Meta.TYPE_WALL&&metaA.getType()==Meta.TYPE_BULLET)
//				meta=metaA;
//			if(meta!=null){
//				
//			}
//		}
		
//		if(metaA.getType()==Meta.TYPE_BULLET)
//			Log.v("start",String.valueOf(((BulletBall)metaA.getShape()).getVelocityX()));
//		if(metaB.getType()==Meta.TYPE_BULLET)
//			Log.v("start",String.valueOf(((BulletBall)metaB.getShape()).getVelocityX()));
		
	}
	
	/**
	 * 碰撞后
	 */
	@Override
	public void endContact(Contact contact) {
		
//		else if(metaA.getType()==Meta.TYPE_WALL&&metaB.getType()==Meta.TYPE_BALL)
//		{
//			rubberBall=(RubberBall)metaB.getShape();
//			rubberBall.getBody().applyForce(frictionVector,new Vector2(rubberBall.getX(),rubberBall.getY()));
//		}
//		else if(metaB.getType()==Meta.TYPE_WALL&&metaA.getType()==Meta.TYPE_BALL)
//		{
//			rubberBall=(RubberBall)metaA.getShape();
//			rubberBall.getBody().applyForce(frictionVector,new Vector2(rubberBall.getX(),rubberBall.getY()));
//		}
		
		
	}

}
