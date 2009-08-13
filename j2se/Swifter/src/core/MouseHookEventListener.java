package core;

import java.awt.Dimension;

import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.MouseHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public class MouseHookEventListener implements HookEventListener{
	private int count=0;
	private Dimension[] dimentions=new Dimension[7];	//记录七个屏幕坐标
	
	public void acceptHookData(HookData hookData) {
		if(hookData.getWParam()==516)			//按右键做记录,只记录七次
		{
			if(count<7)
			{
				System.out.println("第"+count+"个坐标已记录");
				dimentions[count++]=new Dimension(((MouseHookData) hookData).getPointX(),((MouseHookData) hookData).getPointY());
				if(count==7)
				{
					System.out.println("七个屏幕坐标已经完成记录，现在您可以使用键盘控制了");
				}
			}
		}
	}
	
	public void reset()
	{
		count=0;
	}
	
	public void importDimensionSeries(String str)
	{
		String[] dimStrs=str.split(",");
		Dimension zeroDim=dimentions[0];
		for(int i=1;i<this.dimentions.length;i++)
		{
			int width=Integer.parseInt(dimStrs[(i-1)*2])+(int)zeroDim.getWidth();
			int height=Integer.parseInt(dimStrs[(i-1)*2+1])+(int)zeroDim.getHeight();
			this.dimentions[i]=new Dimension(width,height);
		}
		this.count=7;
	}

	public Dimension[] getDimentions() {
		return dimentions;
	}
	
	public String getDimensionSeries()
	{
		Dimension zeroDim=dimentions[0];
		String seriesStr="";
		for(int i=1;i<dimentions.length;i++)
		{
			Dimension dim=dimentions[i];
			if(i<dimentions.length-1)
			{
				seriesStr+=(int)(dim.getWidth()-zeroDim.getWidth())+","+(int)(dim.getHeight()-zeroDim.getHeight())+",";
			}
			else
			{
				seriesStr+=(int)(dim.getWidth()-zeroDim.getWidth())+","+(int)(dim.getHeight()-zeroDim.getHeight());
			}
		}
		return seriesStr;
	}
	
	public void printDimensionSeries(){
		System.out.println("请您记住以下数字序列以方便下次使用(只适用于当前的屏幕分辨率):");
		Dimension zeroDim=dimentions[0];
		System.out.println("相对于"+(int)zeroDim.getWidth()+","+(int)zeroDim.getHeight());
		System.out.println(getDimensionSeries());
	}
	
	
}