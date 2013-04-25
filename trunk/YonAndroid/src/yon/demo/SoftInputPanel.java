package yon.demo;

import android.content.Context;
import android.text.InputType;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;

public abstract class SoftInputPanel {

	private View inputView;
	private EditText inputText;
	private InputMethodManager imm;

	public SoftInputPanel(Context context) {
		LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		inputView = inflater.inflate(R.layout.input, null);

		init();

		imm = ((InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE));
	}
	
	void init()
    {
        Button ok=(Button) inputView.findViewById(R.id.ok);
        Button btnCancel=(Button) inputView.findViewById(R.id.cancel);
        inputText = (EditText) inputView.findViewById(R.id.entry);
        inputView.setVisibility(View.GONE);

    	ok.setOnClickListener(new OnClickListener() {
    		
    		public void onClick(View v) {
    			//nativeDoInput(inputText.getText().toString());
    			//closeInput();
    			completeInput(inputText.getText().toString());
    			setVisible(false);
    		}
    	});
        
        btnCancel.setOnClickListener(new OnClickListener() {
    		
    		public void onClick(View v) {
    			//closeInput();
    			setVisible(false);
    		}
    		
    	});  
        
    }
	
	public View getView(){
		return inputView;
	}
	
	public void setVisible(boolean on)
	{
		if(on)
		{
			inputView.setVisibility(View.VISIBLE);
			inputView.requestFocus();
			inputText.requestFocus();
			imm.showSoftInput(inputText, 0);  
			
			Log.i("SoftInputPanel","show input");
		}
		else
		{
			inputView.setVisibility(View.GONE);
			inputText.clearFocus();
			inputView.clearFocus();
			imm.hideSoftInputFromWindow(inputText.getWindowToken(), 0);
			
			Log.i("SoftInputPanel","hide input");
		}
	}
	
	public abstract void completeInput(String text);

}
