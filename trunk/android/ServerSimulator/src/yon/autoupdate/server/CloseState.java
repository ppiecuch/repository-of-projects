package yon.autoupdate.server;

import yon.autoupdate.core.BaseState;

public class CloseState extends BaseState {

	@Override
	public void onSetup() {
		logger.info("Íê³É¹Ø±Õ£¡\r\n");
	}

	@Override
	public void onShutdown() {
		
	}

}
