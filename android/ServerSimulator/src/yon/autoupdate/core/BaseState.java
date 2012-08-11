package yon.autoupdate.core;

import yon.autoupdate.debug.Logger;

public abstract class BaseState implements State {

	protected Logger logger;

	public void setLogger(Logger logger) {
		this.logger = logger;
	}
	
}
