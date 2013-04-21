package beta03;

import core.Task;

public abstract class Command implements Task {
	
	public static final int TYPE_KEY=1;
	public static final int TYPE_LEFT_MOUSE=2;
	public static final int TYPE_RIGHT_MOUSE=3;
	public static final int TYPE_CALL=4;
	public static final int TYPE_CHECK_COLOR=5;
	public static final int TYPE_SEND_UDP_MSG=6;
	public static final int TYPE_SHUT_DOWN=7;
	
	protected int type;
	protected long delay;

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public long getDelay() {
		return delay;
	}

	public void setDelay(long delay) {
		this.delay = delay;
	}
	
	

}
