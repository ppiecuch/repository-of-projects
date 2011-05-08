package com.temp;

import javax.microedition.khronos.opengles.GL10;

import org.anddev.andengine.engine.Engine;
import org.anddev.andengine.engine.camera.Camera;
import org.anddev.andengine.engine.options.EngineOptions;
import org.anddev.andengine.engine.options.EngineOptions.ScreenOrientation;
import org.anddev.andengine.engine.options.resolutionpolicy.RatioResolutionPolicy;
import org.anddev.andengine.entity.scene.Scene;
import org.anddev.andengine.entity.scene.background.ColorBackground;
import org.anddev.andengine.entity.scene.menu.MenuScene;
import org.anddev.andengine.entity.scene.menu.MenuScene.IOnMenuItemClickListener;
import org.anddev.andengine.entity.scene.menu.item.IMenuItem;
import org.anddev.andengine.entity.scene.menu.item.SpriteMenuItem;
import org.anddev.andengine.opengl.texture.Texture;
import org.anddev.andengine.opengl.texture.TextureOptions;
import org.anddev.andengine.opengl.texture.region.TextureRegion;
import org.anddev.andengine.opengl.texture.region.TextureRegionFactory;
import org.anddev.andengine.ui.activity.BaseGameActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;

public class MainMenu extends BaseGameActivity  implements IOnMenuItemClickListener{
	
	public static final int CAMERA_HEIGHT = 720;
	public static final int CAMERA_WIDTH = 480;
	public static final int POOL_HEIGHT = 520;
	public static final int POOL_WIDTH = 320;
	
	
	protected static final int MENU_RESET = 0;
	protected static final int MENU_QUIT = MENU_RESET + 1;
	
	protected Camera mCamera;

	protected Scene mMainScene;

	private Texture mTexture;
	private TextureRegion mFaceTextureRegion;

	protected MenuScene mMenuScene;

	private Texture mMenuTexture;
	protected TextureRegion mMenuResetTextureRegion;
	protected TextureRegion mMenuQuitTextureRegion;

	@Override
	public void onLoadComplete() {
		
	}

	@Override
	public Engine onLoadEngine() {
		this.mCamera = new Camera(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
		return new Engine(new EngineOptions(true, ScreenOrientation.PORTRAIT, new RatioResolutionPolicy(CAMERA_WIDTH, CAMERA_HEIGHT), this.mCamera));
	}

	@Override
	public void onLoadResources() {
		this.mTexture = new Texture(64, 64, TextureOptions.BILINEAR_PREMULTIPLYALPHA);
		this.mFaceTextureRegion = TextureRegionFactory.createFromAsset(this.mTexture, this, "gfx/face_box_menu.png", 0, 0);
		this.mEngine.getTextureManager().loadTexture(this.mTexture);

		this.mMenuTexture = new Texture(256, 128, TextureOptions.BILINEAR_PREMULTIPLYALPHA);
		this.mMenuResetTextureRegion = TextureRegionFactory.createFromAsset(this.mMenuTexture, this, "gfx/menu_reset.png", 0, 0);
		this.mMenuQuitTextureRegion = TextureRegionFactory.createFromAsset(this.mMenuTexture, this, "gfx/menu_quit.png", 0, 50);
		this.mEngine.getTextureManager().loadTexture(this.mMenuTexture);
		
	}

	@Override
	public Scene onLoadScene() {
		this.mMainScene = new Scene(1);
		this.mMainScene.setBackground(new ColorBackground(0.09804f, 0.6274f, 0.8784f));
		createMenuScene();
		this.mMainScene.setChildScene(this.mMenuScene, false, true, true);
		return this.mMainScene;
	}
	
	
	protected void createMenuScene() {
		this.mMenuScene = new MenuScene(this.mCamera);

		final SpriteMenuItem resetMenuItem = new SpriteMenuItem(MENU_RESET, this.mMenuResetTextureRegion);
		resetMenuItem.setBlendFunction(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);
		this.mMenuScene.addMenuItem(resetMenuItem);

		final SpriteMenuItem quitMenuItem = new SpriteMenuItem(MENU_QUIT, this.mMenuQuitTextureRegion);
		quitMenuItem.setBlendFunction(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);
		this.mMenuScene.addMenuItem(quitMenuItem);

		this.mMenuScene.buildAnimations();

		this.mMenuScene.setBackgroundEnabled(false);
		
		this.mMenuScene.setOnMenuItemClickListener(this);
	}
	
	@Override
	public boolean onMenuItemClicked(final MenuScene pMenuScene, final IMenuItem pMenuItem, final float pMenuItemLocalX, final float pMenuItemLocalY) {
		switch(pMenuItem.getID()) {
			case MENU_RESET:
				/* Restart the animation. */
//				this.mMainScene.reset();
//
//				/* Remove the menu and reset it. */
//				this.mMainScene.clearChildScene();
//				this.mMenuScene.reset();
				startActivity(new Intent(this,Game.class));
				return true;
			case MENU_QUIT:
				/* End Activity. */
				this.finish();
				return true;
			default:
				return false;
		}
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if(keyCode==KeyEvent.KEYCODE_BACK)
		{
			this.finish();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}
}