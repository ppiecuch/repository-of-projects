package yon.autoupdate.ui;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;

public abstract class ConfirmDialog extends Builder {

	public ConfirmDialog(Context context) {
		super(context);
	}
	
	public void setPositiveString(String text){
		this.setPositiveButton(text,new DialogInterface.OnClickListener(){
			public void onClick(DialogInterface dialog, int which) {
				if(onClickPositive())
					dialog.dismiss();
			}
		});
	}
	
	public void setNegativeString(String text){
		this.setNegativeButton(text,new DialogInterface.OnClickListener(){
			public void onClick(DialogInterface dialog, int which) {
				if(onClickNegative())
					dialog.dismiss();//TODO 也许需要换成dialog.cancel()
			}
		});
	}
	
	public abstract boolean onClickPositive();
	public abstract boolean onClickNegative();

}
