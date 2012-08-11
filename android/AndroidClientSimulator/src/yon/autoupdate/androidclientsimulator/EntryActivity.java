package yon.autoupdate.androidclientsimulator;

import yon.autoupdate.debug.Logger;
import yon.autoupdate.net.ClientSocket;
import yon.autoupdate.ui.ConfirmDialog;
import yon.autoupdate.util.Util;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.text.format.Formatter;
import android.util.Log;
import android.app.Activity;
import android.content.Intent;

public class EntryActivity extends Activity implements Constant {

	private static final String TAG = "EntryActivity";
	
	private static final String KEY_IP="HOST_IP";
	private static final String KEY_PORT="HOST_PORT";
	
	private GameApplication application;
	private Logger logger;
	private EntryHandler handler;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_entry);
		
		this.application=(GameApplication)getApplication();
		this.logger=application.getLogger();
		this.handler=new EntryHandler();
		
		ClientSocket.setLogger(logger);
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		Log.d(TAG, "resume");
		if(Util.isNetworkAvailable(this)==false)
		{
			ConfirmDialog confirmDialog=new ConfirmDialog(this) {
				
				@Override
				public boolean onClickPositive() {
					startActivity(new Intent(Settings.ACTION_WIRELESS_SETTINGS));//进入无线网络配置界面
					return true;
				}
				
				@Override
				public boolean onClickNegative() {
					finish();
					return true;
				}
			};
			confirmDialog.setPositiveString(getString(R.string.confirm_dialog_positive));
			confirmDialog.setNegativeString(getString(R.string.confirm_dialog_negetive));
			confirmDialog.setTitle(getString(R.string.confirm_dialog_title_network));
			confirmDialog.setMessage(getString(R.string.confirm_dialog_content_network));
			confirmDialog.show();
			
			return;
		}
		else
		{
			Log.d(TAG, "连接正常");
		}
		
		if(Util.isSDCardAvailable()==false) {
			ConfirmDialog confirmDialog=new ConfirmDialog(this) {
				
				@Override
				public boolean onClickPositive() {
					finish();
					return true;
				}
				
				@Override
				public boolean onClickNegative() {
					return true;
				}
			};
			confirmDialog.setPositiveString(getString(R.string.confirm_dialog_positive));
			confirmDialog.setTitle(getString(R.string.confirm_dialog_title_sd));
			confirmDialog.setMessage(getString(R.string.confirm_dialog_content_sd));
			confirmDialog.show();
			
			return;
		}
		else
		{
			Log.d(TAG, "SD卡正常");
			Log.d(TAG,String.valueOf(Util.getVersionCode(this)));
			Log.d(TAG,Util.getVersionName(this));
			//TODO 检测第一次启动
			
			logger.debug("IP:"+Util.getLocalIP());
			logger.debug("MAC:"+Util.getLocalMacAddress(this));
			logger.debug("DNS1:"+Formatter.formatIpAddress(Util.getDHCPInfo(this).dns1));
			logger.debug("DNS2:"+Formatter.formatIpAddress(Util.getDHCPInfo(this).dns2));
			logger.debug("GateWay:"+Formatter.formatIpAddress(Util.getDHCPInfo(this).gateway));
			checkClientVersion();
		}
	}
	
	private void checkClientVersion(){
		CheckVersionStreamProcessor processor=new CheckVersionStreamProcessor();
		processor.setLogger(logger);
		CheckVersionCallback callback=new CheckVersionCallback(handler);
		ClientSocket.connect(application.getString(KEY_IP), application.getInteger(KEY_PORT), 2000, processor, callback);
	}
	
	
	private class EntryHandler extends Handler{
		@Override
		public void handleMessage(Message msg) {
			switch(msg.what)
			{
			case MSG_NETWORK_TIMEOUT:
				{
					ConfirmDialog confirmDialog=new ConfirmDialog(EntryActivity.this) {
						
						@Override
						public boolean onClickPositive() {
							finish();
							return true;
						}
						
						@Override
						public boolean onClickNegative() {
							return true;
						}
					};
					confirmDialog.setPositiveString(getString(R.string.confirm_dialog_positive));
					confirmDialog.setTitle(getString(R.string.confirm_dialog_title_timeout));
					confirmDialog.setMessage(getString(R.string.confirm_dialog_content_timeout));
					confirmDialog.show();
				}
				break;
			case MSG_NETWORK_FAILED:
				{
					ConfirmDialog confirmDialog=new ConfirmDialog(EntryActivity.this) {
						
						@Override
						public boolean onClickPositive() {
							finish();
							return true;
						}
						
						@Override
						public boolean onClickNegative() {
							return true;
						}
					};
					confirmDialog.setPositiveString(getString(R.string.confirm_dialog_positive));
					confirmDialog.setTitle(getString(R.string.confirm_dialog_title_failed));
					confirmDialog.setMessage(getString(R.string.confirm_dialog_content_failed));
					confirmDialog.show();
				}
				break;
			default:
				logger.warn("unknown msg");
			}
		}
	}
	
	
}