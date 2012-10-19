

/************************************************************************************************* 
* 将UTF8编码转换成Unicode（UCS-2）编码 
* 参数： 
* char* pInput 指向输入字符串（以'\0'结尾）的指针 
* char** ppOutput 指向输出字符串指针的指针 
* 返回值： 
* 返回转换后的Unicode字符串的字节数，如果出错则返回-1 
* 注意： 
* 1. UTF8没有字节序问题，但是Unicode字符有字节序， 
* 字节序分为大端（Big Endian）和小端（Little Endian）两种， 
* 在Intel处理器中采用小端法表示，因此本例中采用小端法表示。（低地址存低位） 
* 2. 在调用本函数后需要手动释放 *ppOutput 指向的内存，否则将会造成内存泄漏。 
**************************************************************************************************/  
int utf8_to_ucs2(const char* pInput, xc::c16** ppOutput)
{  
	int outputSize = 0; //记录转换后的Unicode字符串的字节数  
	*ppOutput = new xc::c16[strlen(pInput) + 1]; //为输出字符串分配足够大的内存空间  
	memset(*ppOutput, 0, strlen(pInput) + 1);  
	char *tmp = (char*)*ppOutput; //临时变量，用于遍历输出字符串  
	while (*pInput)  
	{  
		if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）  
		{  
			*tmp = *pInput;  
			tmp++;  
			*tmp = 0; //小端法表示，在高地址填补0  
		}  
		else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符  
		{  
			char high = *pInput;  
			pInput++;  
			char low = *pInput;  
			if ((low & 0xC0) != 0x80) //检查是否为合法的UTF8字符表示  
			{  
				return -1; //如果不是则报错  
			}  

			*tmp = (high << 6) + (low & 0x3F);  
			tmp++;  
			*tmp = (high >> 2) & 0x07;  
		}  
		else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符  
		{  
			char high = *pInput;  
			pInput++;  
			char middle = *pInput;  
			pInput++;  
			char low = *pInput;  
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))  
			{  
				return -1;  
			}  

			*tmp = (middle << 6) + (low & 0x7F);  
			tmp++;  
			*tmp = (high << 4) + ((middle >> 2) & 0x0F);  

		}  
		else //对于其他字节数的UTF8字符不进行处理  
		{  
			return -1;  
		}  

		pInput ++;  
		tmp ++;  
		outputSize += 2;  
	}  

	*tmp = 0;  
	tmp++;  
	*tmp = 0;  
	return outputSize;  
}  
int strlenc16(const xc::c16* str)
{
	int l = 0;
	while ( *str ++ )
		l ++ ;

	return l;
}

xc::c16* strcpyc16(xc::c16* dest, xc::c16* src)
{
	xc::c16* dest_bak = dest;

	while (*src)
	{
		*dest ++ = *src ++;
	}

	*dest = 0;

	return dest_bak;
}
