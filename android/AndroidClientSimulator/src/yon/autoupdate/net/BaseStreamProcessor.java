package yon.autoupdate.net;

import yon.autoupdate.debug.Logger;

public abstract class BaseStreamProcessor implements StreamProcessor {

	protected Logger logger;

	public void setLogger(Logger logger) {
		this.logger = logger;
	}
}
