package module;

import java.awt.Point;
import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.math.BigDecimal;

public class Operation implements Externalizable {

	private static final long serialVersionUID = 1L;
	public static final int MODE_KEYBOARD=1;
	public static final int MODE_MOUSE=2;
	public static final int MODE_UNKNOWN=3;

	private Integer instruction = null; 	//键盘按键
	private Point point; 					//鼠标左击位置
	private Long delay;					//延迟时间

	public void readExternal(ObjectInput in) throws IOException,
			ClassNotFoundException {
		decode(in.readLine());
	}

	public void writeExternal(ObjectOutput out) throws IOException {
		out.writeBytes(this.encode());
		out.writeChar('\n');
	}

	public Long getDelay() {
		return delay;
	}

	public void setDelay(Long delay) {
		this.delay = delay;
	}

	public Integer getInstruction() {
		return instruction;
	}

	public void setInstruction(Integer instruction) {
		this.instruction = instruction;
	}

	public Point getPoint() {
		return point;
	}

	public void setPoint(Point point) {
		this.point = point;
	}

	@Override
	public String toString() {
		return encode();
	}

	private void decode(String info) {
		if (info != null && info.length() != 0) {
			String str[] = info.split("@");
			if (str.length == 2) {
				this.delay = (long)(round((Double.parseDouble(str[1].trim())*1000),0));
				String[] sstr = str[0].split(",");
				if (sstr.length == 2) {
					this.point = new Point(Integer.parseInt(sstr[0].trim()),
							Integer.parseInt(sstr[1].trim()));
				} else if (sstr.length == 1) {
					this.instruction = Integer.parseInt(sstr[0].trim());
				} else {
					this.delay = null;
				}
			}
		}
	}

	private String encode() {
		if (instruction == null) {
			return point.x + "," + point.y + "@" + round((double)delay/1000,1);
		} else {
			return instruction + "@" + round((double)delay/1000,1);
		}
	}
	
	public int getMode()
	{
		if(this.instruction==null&&this.point!=null)
		{
			return MODE_MOUSE;
		}
		else if(this.instruction!=null&&this.point==null)
		{
			return MODE_KEYBOARD;
		}
		return MODE_UNKNOWN;
	}

	/**
	 * 
	 * 提供精确的小数位四舍五入处理。
	 * 
	 * @param v 需要四舍五入的数字
	 * @param scale 小数点后保留几位
	 * @return 四舍五入后的结果
	 * 
	 */
	public static double round(double v, int scale) {
		if (scale < 0) {
			throw new IllegalArgumentException(
					"The scale must be a positive integer or zero");
		}
		BigDecimal b = new BigDecimal(Double.toString(v));
		BigDecimal one = new BigDecimal("1");
		return b.divide(one, scale, BigDecimal.ROUND_HALF_UP).doubleValue();
	}

}
