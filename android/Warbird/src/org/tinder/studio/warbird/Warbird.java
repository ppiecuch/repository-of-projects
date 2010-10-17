package org.tinder.studio.warbird;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

public class Warbird extends Activity implements OnClickListener {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        View startButton=findViewById(R.id.start_button);
        startButton.setOnClickListener(this);
        View exitButton=findViewById(R.id.exit_button);
        exitButton.setOnClickListener(this);
    }

	@Override
	public void onClick(View v) {
		switch(v.getId())
		{
		case R.id.start_button:
			Log.d(this.toString(),"click on start button");
			startActivity(new Intent(this,Game.class));
			break;
		case R.id.exit_button:
			Log.d(this.toString(),"click on exit button");
			finish();
			break;
		}
		
	}
}