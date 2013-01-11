package yon;
import java.util.LinkedList;
import java.util.List;
import android.app.Activity;
import android.app.Application;
import android.util.Log;

public class SysApplication extends Application {
	private static final String TAG="SysApplication";
	private List<Activity> mList = new LinkedList<Activity>();
	private static SysApplication instance;

	private SysApplication() {
	}

	public synchronized static SysApplication getInstance() {
		if (null == instance) {
			instance = new SysApplication();
		}
		return instance;
	}

	// add Activity
	public void addActivity(Activity activity) {
		mList.add(activity);
	}

	public void exit() {
		try {
			for (Activity activity : mList) {
				if (activity != null)
					activity.finish();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			Log.d(TAG,"System.exit(0)");
			//System.exit(0);
		}
	}

	@Override
	public void onLowMemory() {
		super.onLowMemory();
		System.gc();
	}

}
