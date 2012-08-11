package yon.autoupdate.debug;

import android.util.Log;

public class ConsoleLogger implements Logger {
	
	private String getCaller() {
		StackTraceElement[] stack = (new Throwable()).getStackTrace();
		return stack[2].getClassName();
	}

	@Override
	public void debug(String str) {
		Log.d(getCaller(),str);
	}

	@Override
	public void info(String str) {
		Log.i(getCaller(),str);
	}

	@Override
	public void warn(String str) {
		Log.w(getCaller(),str);
	}

	@Override
	public void error(String str) {
		Log.e(getCaller(),str);
	}

}
