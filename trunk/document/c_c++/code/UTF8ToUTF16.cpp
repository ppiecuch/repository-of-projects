

/************************************************************************************************* 
* ��UTF8����ת����Unicode��UCS-2������ 
* ������ 
* char* pInput ָ�������ַ�������'\0'��β����ָ�� 
* char** ppOutput ָ������ַ���ָ���ָ�� 
* ����ֵ�� 
* ����ת�����Unicode�ַ������ֽ�������������򷵻�-1 
* ע�⣺ 
* 1. UTF8û���ֽ������⣬����Unicode�ַ����ֽ��� 
* �ֽ����Ϊ��ˣ�Big Endian����С�ˣ�Little Endian�����֣� 
* ��Intel�������в���С�˷���ʾ����˱����в���С�˷���ʾ�����͵�ַ���λ�� 
* 2. �ڵ��ñ���������Ҫ�ֶ��ͷ� *ppOutput ָ����ڴ棬���򽫻�����ڴ�й©�� 
**************************************************************************************************/  
int utf8_to_ucs2(const char* pInput, xc::c16** ppOutput)
{  
	int outputSize = 0; //��¼ת�����Unicode�ַ������ֽ���  
	*ppOutput = new xc::c16[strlen(pInput) + 1]; //Ϊ����ַ��������㹻����ڴ�ռ�  
	memset(*ppOutput, 0, strlen(pInput) + 1);  
	char *tmp = (char*)*ppOutput; //��ʱ���������ڱ�������ַ���  
	while (*pInput)  
	{  
		if (*pInput > 0x00 && *pInput <= 0x7F) //�����ֽ�UTF8�ַ���Ӣ����ĸ�����֣�  
		{  
			*tmp = *pInput;  
			tmp++;  
			*tmp = 0; //С�˷���ʾ���ڸߵ�ַ�0  
		}  
		else if (((*pInput) & 0xE0) == 0xC0) //����˫�ֽ�UTF8�ַ�  
		{  
			char high = *pInput;  
			pInput++;  
			char low = *pInput;  
			if ((low & 0xC0) != 0x80) //����Ƿ�Ϊ�Ϸ���UTF8�ַ���ʾ  
			{  
				return -1; //��������򱨴�  
			}  

			*tmp = (high << 6) + (low & 0x3F);  
			tmp++;  
			*tmp = (high >> 2) & 0x07;  
		}  
		else if (((*pInput) & 0xF0) == 0xE0) //�������ֽ�UTF8�ַ�  
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
		else //���������ֽ�����UTF8�ַ������д���  
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
