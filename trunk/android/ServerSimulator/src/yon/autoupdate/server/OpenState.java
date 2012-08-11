package yon.autoupdate.server;

import yon.autoupdate.core.BaseState;
import yon.autoupdate.net.ConnectionManager;


public class OpenState extends BaseState {

	@Override
	public void onSetup() {
		logger.info("±¾µØIP£º"+ConnectionManager.getInstance().getLocalIP()+"\r\n");
	}

	@Override
	public void onShutdown() {
		
	}

}
