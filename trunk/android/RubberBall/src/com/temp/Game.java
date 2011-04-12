package com.temp;

import org.anddev.andengine.engine.Engine;
import org.anddev.andengine.engine.camera.Camera;
import org.anddev.andengine.engine.handler.physics.PhysicsHandler;
import org.anddev.andengine.engine.options.EngineOptions;
import org.anddev.andengine.engine.options.EngineOptions.ScreenOrientation;
import org.anddev.andengine.engine.options.resolutionpolicy.RatioResolutionPolicy;
import org.anddev.andengine.entity.primitive.Rectangle;
import org.anddev.andengine.entity.scene.Scene;
import org.anddev.andengine.entity.scene.Scene.IOnSceneTouchListener;
import org.anddev.andengine.entity.scene.background.ColorBackground;
import org.anddev.andengine.entity.shape.Shape;
import org.anddev.andengine.entity.sprite.AnimatedSprite;
import org.anddev.andengine.extension.physics.box2d.PhysicsConnector;
import org.anddev.andengine.extension.physics.box2d.PhysicsFactory;
import org.anddev.andengine.extension.physics.box2d.PhysicsWorld;
import org.anddev.andengine.extension.physics.box2d.util.Vector2Pool;
import org.anddev.andengine.input.touch.TouchEvent;
import org.anddev.andengine.opengl.texture.Texture;
import org.anddev.andengine.opengl.texture.TextureOptions;
import org.anddev.andengine.opengl.texture.region.TextureRegionFactory;
import org.anddev.andengine.opengl.texture.region.TiledTextureRegion;
import org.anddev.andengine.sensor.accelerometer.AccelerometerData;
import org.anddev.andengine.sensor.accelerometer.IAccelerometerListener;
import org.anddev.andengine.ui.activity.BaseGameActivity;

import android.hardware.SensorManager;
import android.util.Log;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.Body;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.BodyDef.BodyType;
import com.temp.sprite.RubberBall;

public class Game extends BaseGameActivity implements IAccelerometerListener,IOnSceneTouchListener {
	
	private Camera mCamera;

	private Texture mTexture;
	private TiledTextureRegion mFaceTextureRegion;
	
	private static final FixtureDef FIXTURE_DEF = PhysicsFactory.createFixtureDef(1, 0.5f, 0.5f);
//	private PhysicsWorld mPhysicsWorld1;
	private PhysicsWorld mPhysicsWorld2;
	
	
	@Override
	public void onLoadComplete() {
		// TODO Auto-generated method stub

	}

	@Override
	public Engine onLoadEngine() {
		this.mCamera = new Camera(0, 0, MainMenu.CAMERA_HEIGHT, MainMenu.CAMERA_WIDTH);
//		return new Engine(new EngineOptions(true, ScreenOrientation.PORTRAIT, new RatioResolutionPolicy(MainMenu.CAMERA_HEIGHT, MainMenu.CAMERA_WIDTH), this.mCamera));
		final EngineOptions engineOptions = new EngineOptions(true, ScreenOrientation.PORTRAIT, new RatioResolutionPolicy(MainMenu.CAMERA_HEIGHT, MainMenu.CAMERA_WIDTH), this.mCamera);
		engineOptions.getTouchOptions().setRunOnUpdateThread(true);
		return new Engine(engineOptions);
	}

	@Override
	public void onLoadResources() {
		this.mTexture = new Texture(64, 32, TextureOptions.BILINEAR_PREMULTIPLYALPHA);
		this.mFaceTextureRegion = TextureRegionFactory.createTiledFromAsset(this.mTexture, this, "gfx/face_circle_tiled.png", 0, 0, 2, 1);
		this.mEngine.getTextureManager().loadTexture(this.mTexture);
		
//		this.enableAccelerometerSensor(this);
	}
	
	@Override
	public boolean onSceneTouchEvent(final Scene pScene, final TouchEvent pSceneTouchEvent) {
		if(this.mPhysicsWorld2 != null) {
			if(pSceneTouchEvent.isActionDown()) {
				Log.d("touch","addFace");
				this.addFace(pSceneTouchEvent.getX(), pSceneTouchEvent.getY());
				return true;
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
		

//		final int centerX = (MainMenu.CAMERA_WIDTH - this.mFaceTextureRegion.getWidth()) / 2;
//		final int centerY = (MainMenu.CAMERA_HEIGHT - this.mFaceTextureRegion.getHeight()) / 2;
//		final RubberBall ball = new RubberBall(centerX, centerY, this.mFaceTextureRegion);
//		final PhysicsHandler physicsHandler = new PhysicsHandler(ball);
//		ball.registerUpdateHandler(physicsHandler);
//		physicsHandler.setVelocity(RubberBall.DEMO_VELOCITY, RubberBall.DEMO_VELOCITY);

//		scene.getLastChild().attachChild(ball);
		
		this.mPhysicsWorld2 = new PhysicsWorld(new Vector2(0, SensorManager.GRAVITY_EARTH), false);

//		this.mPhysicsWorld1 = new PhysicsWorld(new Vector2(0, 0), false);
		final Shape ground = new Rectangle(0, MainMenu.CAMERA_WIDTH - 2, MainMenu.CAMERA_HEIGHT, 2);
		final Shape roof = new Rectangle(0, 0, MainMenu.CAMERA_HEIGHT, 2);
		final Shape left = new Rectangle(0, 0, 2, MainMenu.CAMERA_WIDTH);
		final Shape right = new Rectangle(MainMenu.CAMERA_HEIGHT - 2, 0, 2, MainMenu.CAMERA_WIDTH);
		
		final FixtureDef wallFixtureDef1 = PhysicsFactory.createFixtureDef(0, 1f, 0f);
//		final FixtureDef wallFixtureDef2 = PhysicsFactory.createFixtureDef(0, 0.5f, 0.5f);
//		PhysicsFactory.createBoxBody(this.mPhysicsWorld1, ground, BodyType.StaticBody, wallFixtureDef1);
//		PhysicsFactory.createBoxBody(this.mPhysicsWorld1, roof, BodyType.StaticBody, wallFixtureDef1);
//		PhysicsFactory.createBoxBody(this.mPhysicsWorld1, left, BodyType.StaticBody, wallFixtureDef1);
//		PhysicsFactory.createBoxBody(this.mPhysicsWorld1, right, BodyType.StaticBody, wallFixtureDef1);
		
		PhysicsFactory.createBoxBody(this.mPhysicsWorld2, ground, BodyType.StaticBody, wallFixtureDef1);
		PhysicsFactory.createBoxBody(this.mPhysicsWorld2, roof, BodyType.StaticBody, wallFixtureDef1);
		PhysicsFactory.createBoxBody(this.mPhysicsWorld2, left, BodyType.StaticBody, wallFixtureDef1);
		PhysicsFactory.createBoxBody(this.mPhysicsWorld2, right, BodyType.StaticBody, wallFixtureDef1);
		
		scene.getFirstChild().attachChild(ground);
		scene.getFirstChild().attachChild(roof);
		scene.getFirstChild().attachChild(left);
		scene.getFirstChild().attachChild(right);

//		scene.registerUpdateHandler(this.mPhysicsWorld1);
		scene.registerUpdateHandler(this.mPhysicsWorld2);
		
		final AnimatedSprite face1;
		final Body body1;
		face1 = new AnimatedSprite(55, 55, this.mFaceTextureRegion);
		body1 = PhysicsFactory.createCircleBody(this.mPhysicsWorld2, face1, BodyType.DynamicBody, FIXTURE_DEF);
//		body1.applyForce(new Vector2(0, -SensorManager.GRAVITY_EARTH),body1.getPosition());
		body1.applyLinearImpulse(new Vector2(0, -SensorManager.GRAVITY_EARTH*2),body1.getPosition());
		
//		body1 = PhysicsFactory.createCircleBody(this.mPhysicsWorld1, face1, BodyType.DynamicBody, FIXTURE_DEF);

		body1.setLinearVelocity(new Vector2(20,0));
		face1.animate(200);
		scene.getLastChild().attachChild(face1);
		this.mPhysicsWorld2.registerPhysicsConnector(new PhysicsConnector(face1, body1, true, true));
		
//		this.mPhysicsWorld1.registerPhysicsConnector(new PhysicsConnector(face1, body1, true, true));
		
		return scene;
	}
	
	private void addFace(float x,float y){
		final Scene scene = this.mEngine.getScene();
		final AnimatedSprite face2;
		final Body body2;
		face2 = new AnimatedSprite(x, y, this.mFaceTextureRegion);
		body2 = PhysicsFactory.createCircleBody(this.mPhysicsWorld2, face2, BodyType.DynamicBody, FIXTURE_DEF);
		face2.animate(200);
		scene.getLastChild().attachChild(face2);
		this.mPhysicsWorld2.registerPhysicsConnector(new PhysicsConnector(face2, body2, true, true));
		
	}

}
