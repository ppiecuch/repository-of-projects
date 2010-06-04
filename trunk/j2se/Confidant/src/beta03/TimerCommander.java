package beta03;

import java.util.Date;

import util.DateUtils;

public class TimerCommander extends Commander {
	
	private Date time;

	public Date getTime() {
		return time;
	}

	public void setTime(Date time) {
		this.time = time;
	}
	
	public void setTime(String time){
		this.time=DateUtils.stringToDate(time,"yyyy-MM-dd HH:mm");
	}
	
	

}
