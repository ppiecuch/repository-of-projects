package org.tinder.studio.warbird;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class Game extends Activity {
	
	private GameView view;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.view=new GameView(this);
		setContentView(view);
		view.requestFocus();
		view.start();
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		Log.d("Game","Pause");
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.d("Game","Destroy");
	}
}
