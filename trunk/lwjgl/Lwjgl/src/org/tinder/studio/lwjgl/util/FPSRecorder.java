package org.tinder.studio.lwjgl.util;
/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-6обнГ09:26:34
 *
 */
public class FPSRecorder {
	
	private static long[] costTimes=new long[100];
	private static int index=0;
	
	public static void push(long time)
	{
		costTimes[index++]=time;
		if(index==costTimes.length)
			index=0;
	}
	
	public static long getAverage()
	{
		long total=0;
		for(int i=0;i<costTimes.length;i++)
			total+=costTimes[i];
		return total/costTimes.length;
	}

}
