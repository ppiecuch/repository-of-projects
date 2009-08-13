package core;

import java.awt.Dimension;

import org.sf.feeling.swt.win32.extension.hook.data.HookData;
import org.sf.feeling.swt.win32.extension.hook.data.MouseHookData;
import org.sf.feeling.swt.win32.extension.hook.listener.HookEventListener;

public class MouseHookEventListener implements HookEventListener{
	private int count=0;
	private Dimension[] dimentions=new Dimension[7];	//��¼�߸���Ļ����
	
	public void acceptHookData(HookData hookData) {
		if(hookData.getWParam()==516)			//���Ҽ�����¼,ֻ��¼�ߴ�
		{
			if(count<7)
			{
				System.out.println("��"+count+"�������Ѽ�¼");
				dimentions[count++]=new Dimension(((MouseHookData) hookData).getPointX(),((MouseHookData) hookData).getPointY());
				if(count==7)
				{
					System.out.println("�߸���Ļ�����Ѿ���ɼ�¼������������ʹ�ü��̿�����");
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
		System.out.println("������ס�������������Է����´�ʹ��(ֻ�����ڵ�ǰ����Ļ�ֱ���):");
		Dimension zeroDim=dimentions[0];
		System.out.println("�����"+(int)zeroDim.getWidth()+","+(int)zeroDim.getHeight());
		System.out.println(getDimensionSeries());
	}
	
	
}