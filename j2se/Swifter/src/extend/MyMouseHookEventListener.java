package extend;

import java.awt.Dimension;
import java.util.LinkedList;
import java.util.List;

public class MyMouseHookEventListener extends MouseHookEventListener {
	private Dimension zeroDimension;
	private List<Dimension> dimensions=new LinkedList<Dimension>();
	private boolean needFetchZeroDimension=false;
	private String currentOffsetSeries="";
	
	public void resetZeroDimension()
	{
		this.needFetchZeroDimension=true;
	}
	
	public void resetDimensions(String dimensionSeries)
	{
		this.dimensions.clear();
		String[] dimStrs=dimensionSeries.split(",");
		for(int i=0;dimStrs!=null&&i<dimStrs.length/2;i++)
		{
			int width=Integer.parseInt(dimStrs[i*2])+(int)zeroDimension.getWidth();
			int height=Integer.parseInt(dimStrs[i*2+1])+(int)zeroDimension.getHeight();
			dimensions.add(new Dimension(width,height));
		}
	}
	
	public String getDimensionSeries()
	{
		String dimSeries="";
		for(Dimension dim:this.dimensions)
		{
			dimSeries=dimSeries+","+(int)(dim.getWidth()-zeroDimension.getWidth())+","+(int)(dim.getHeight()-zeroDimension.getHeight());
		}
		if(dimSeries.length()>0)
		{
			dimSeries=dimSeries.substring(1);
		}
		return dimSeries;
	}

	@Override
	protected void doLeftPressed(int x, int y) {}

	@Override
	protected void doLeftReleased(int x, int y) {}

	@Override
	protected void doMiddlePressed(int x, int y) {}

	@Override
	protected void doMiddleReleased(int x, int y) {}

	@Override
	protected void doRightPressed(int x, int y) {
		if(this.needFetchZeroDimension)
		{
			this.zeroDimension=new Dimension(x,y);
			resetDimensions(currentOffsetSeries);
			this.needFetchZeroDimension=false;
			System.out.println("幸运星位置已获取，关闭重置模式，\r\n现在你可以使用小键盘上的12345来实现鼠标事件模拟，如果你需要重新选择请按F11");
		}
	}

	@Override
	protected void doRightReleased(int x, int y) {}

	public void setCurrentOffsetSeries(String currentOffsetSeries) {
		this.currentOffsetSeries = currentOffsetSeries;
	}

	public List<Dimension> getDimensions() {
		return dimensions;
	}
	
	

}
