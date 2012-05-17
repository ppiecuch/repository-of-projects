package com.demo.list;

import java.util.ArrayList;
import java.util.List;

import com.demo.list.examples.Example00;
import com.demo.list.examples.Example01;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

public class AndroidListActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        ListView lv=new ListView(this);
        ArrayAdapter<String> ad=new ArrayAdapter<String>(this,R.layout.list_row,getData());
        lv.setAdapter(ad);

        lv.setOnItemClickListener(new OnItemClickListener() {

			public void onItemClick(AdapterView<?> parent,  View v, int position, long id) {
				//Log.d("test",String.valueOf(position));
				//setTitle(parent.getItemAtPosition(position).toString());
				switch(position)
				{
				case 0:
					{
						Intent intent=new Intent(AndroidListActivity.this,Example00.class);
						startActivity(intent);
					}
					break;
				case 1:
					{
						Intent intent=new Intent(AndroidListActivity.this,Example01.class);
						startActivity(intent);
					}
					break;
				}
			}
		});
        
        setContentView(lv);
   }
    
    private List<String> getData(){
        
        List<String> data = new ArrayList<String>();
        data.add("Example00");
        data.add("Example01");
         
        return data;
    }
}