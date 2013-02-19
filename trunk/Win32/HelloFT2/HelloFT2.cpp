// HelloFT2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h >

int _tmain(int argc, _TCHAR* argv[])
{
	FT_Library	pFTLib		= NULL;
	FT_Face		pFTFace		= NULL;
	FT_Error	error		= 0;
	error = FT_Init_FreeType(&pFTLib);
	if(error)
	{
		pFTLib = 0;
		printf("There is some error when Init Library...");
		return -1;
	}

	error = FT_New_Face(pFTLib, "C:\\WINDOWS\\Fonts\\arial.ttf", 0, &pFTFace);
	if(!error)
	{
		FT_Set_Char_Size(pFTFace, 16<<6, 16<<6, 300, 300);
		FT_Glyph	glyph;
		FT_Load_Glyph(pFTFace, FT_Get_Char_Index(pFTFace, 67), FT_LOAD_DEFAULT);
		error = FT_Get_Glyph(pFTFace->glyph, &glyph);
		if(!error)
		{
			FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
			FT_BitmapGlyph	bitmap_glyph = (FT_BitmapGlyph)glyph;
			FT_Bitmap&	bitmap = bitmap_glyph->bitmap;
			for(int i=0; i<bitmap.rows; ++i)
			{
				for(int j=0; j<bitmap.width; ++j)
				{
					printf("%d", bitmap.buffer[i*bitmap.width+j]?1:0);
				}
				printf("\n");
			}
			FT_Done_Glyph(glyph);
			glyph = NULL;
		}
		FT_Done_Face(pFTFace);
		pFTFace = NULL;
	}

	FT_Done_FreeType(pFTLib);
	pFTLib = NULL;

	system("pause");
	return 0;
}

