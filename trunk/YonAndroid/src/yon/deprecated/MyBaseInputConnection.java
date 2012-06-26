package yon.deprecated;

import android.util.Log;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;

/**
 * BaseInputConnection�൱��һ��InputMethodService��View֮���һ��ͨ����
 * ÿ��InputMethodService����һ�����ʱ���������BaseInputConnection��commitText�������ѽ�����ݳ����� 
 * copy from http://king523103.iteye.com/blog/1190100
 * @author Micheal
 *
 */
public class MyBaseInputConnection extends BaseInputConnection {
	
	public String result="";

	public MyBaseInputConnection(View targetView, boolean fullEditor) {
		super(targetView, fullEditor);
	}
	
	//���뷨�������ͨ������������������ս��������ġ�
	@Override
	public boolean commitText(CharSequence text, int newCursorPosition) {
		result=text.toString();
		Log.i("Connection",result);
		return true;
	}

}
