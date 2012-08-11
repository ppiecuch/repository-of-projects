package yon.autoupdate.net;

public interface Callback {
	public void doTimeout();
	public void doFailed();
	public void doCompleted();
}
