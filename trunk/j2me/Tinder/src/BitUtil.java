
public class BitUtil {
	
	/**
	 * 获取最高比特位,范围为15-(-1)
	 * @param num
	 * @return
	 */
	public static int highestBit(int num)
	{
		if(num<0)return 15;
		if(num>=16384)return 14;
		if(num>=8192)return 13;
		if(num>=4096)return 12;
		if(num>=2048)return 11;
		if(num>=1024)return 10;
		if(num>=512)return 9;
		if(num>=256)return 8;
		if(num>=128)return 7;
		if(num>=64)return 6;
		if(num>=32)return 5;
		if(num>=16)return 4;
		if(num>=8)return 3;
		if(num>=4)return 2;
		if(num>=2)return 1;
		if(num>=1)return 0;
		return -1;
	}

}
