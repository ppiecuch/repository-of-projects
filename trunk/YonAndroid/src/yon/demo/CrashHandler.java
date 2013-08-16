package yon.demo;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Handler;
import android.os.Looper;

public class CrashHandler implements Thread.UncaughtExceptionHandler {
	public static final String TAG = CrashHandler.class.getSimpleName();
	private static CrashHandler INSTANCE = new CrashHandler();
	private Context mContext;

	// private Thread.UncaughtExceptionHandler mDefaultHandler;

	private CrashHandler() {
	}

	public static CrashHandler getInstance() {
		return INSTANCE;
	}

	public void init(Context ctx) {
		mContext = ctx;
		// mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
		Thread.setDefaultUncaughtExceptionHandler(this);
	}

	@Override
	public void uncaughtException(Thread thread, Throwable ex) {
		// if (!handleException(ex) && mDefaultHandler != null) {
		// mDefaultHandler.uncaughtException(thread, ex);
		// } else {
		// android.os.Process.killProcess(android.os.Process.myPid());
		// System.exit(10);
		// }
		System.out.println("uncaughtException");

		new Thread() {
			@Override
			public void run() {
				Looper.prepare();
				new AlertDialog.Builder(mContext).setTitle("��ʾ")
						.setCancelable(false).setMessage("���������...")
						.setNeutralButton("��֪����", new OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								System.exit(0);
							}
						}).create().show();
				Looper.loop();
			}
		}.start();
	}

	/**
	 * �Զ��������,�ռ�������Ϣ ���ʹ��󱨸�Ȳ������ڴ����. �����߿��Ը����Լ���������Զ����쳣�����߼�
	 * 
	 * @param ex
	 * @return true:��������˸��쳣��Ϣ;���򷵻�false
	 */
	private boolean handleException(Throwable ex) {
		if (ex == null) {
			return true;
		}
		/*new Handler(Looper.getMainLooper()).post(new Runnable() {
			@Override
			public void run() {
				new AlertDialog.Builder(mContext).setTitle("��ʾ")
						.setMessage("���������...").setNeutralButton("��֪����", null)
						.create().show();
			}
		});*/

		return true;
	}
}
