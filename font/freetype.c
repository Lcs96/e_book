#include <fonts_manager.h>
#include <config.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H



static T_FontOpr g_tFreeTypeOpr = {
	.name      = "FreeType";
	.FontInit  = FreeTypeInit;
	.ShowFonts = FreeTypePut;
};

static FT_Library g_tLibrary;
static FT_Face g_tFace;
static FT_GlyphSlot g_tSlot;

int FreeTypeInit(char *pcFontFile, unsigned int size)
{
	int iError;
	iError = FT_Init_FreeType(&g_tLibrary );
	if (iError)
		{
			printf("can't init freetype\n");
			return -1;
		}
	iError = FT_New_Face(g_tLibrary, pcFontFile, 0, &g_tFace);
	if (iError)
		{
			printf("can't new face\n");
			return -1;
		}
	g_tSlot = g_tFace->glyph;
	iError = FT_Set_Pixel_Sizes(g_tFace, size, 0);
	if (iError)
		{
			printf("FT_Set_Pixel_Sizes failed\n");
			return -1;
		}

	return 0;

	
}

int FreeTypePut(unsigned int uiCode, PT_FontInf ptFontInf)
{
	int iError;
	int iPenX = ptFontInf->iCurOriginX;
	int iPenY = ptFontInf->iCurOriginY;
	FT_Vector tPen;

	tPen.x = iPenX;
	tPen.y = iPenY;
	FT_Set_Transform(g_tFace, 0, &tPen);
	iError = FT_Load_Char(g_tFace, uiCode, FT_LOAD_RENDER );
	if (iError)
	{
		printf("FT_Load_Char error for code : 0x%x\n", dwCode);
		return -1;
	}
	ptFontInf->iXLeft    = iPenX + g_tSlot->bitmap_left;
	ptFontInf->iYTop     = iPenY - g_tSlot->bitmap_top;
	ptFontInf->iXMax     = ptFontBitMap->iXLeft + g_tSlot->bitmap.width;
	ptFontInf->iYMax     = ptFontBitMap->iYTop  + g_tSlot->bitmap.rows;
	ptFontInf->iBpp      = 8;
	ptFontInf->pucBuffer = g_tSlot->bitmap.buffer;
	
	ptFontInf->iNextOriginX = iPenX + g_tSlot->advance.x / 64;
	ptFontInf->iNextOriginY = iPenY;

	//DBG_PRINTF("iXLeft = %d, iYTop = %d, iXMax = %d, iYMax = %d, iNextOriginX = %d, iNextOriginY = %d\n", ptFontBitMap->iXLeft, ptFontBitMap->iYTop, ptFontBitMap->iXMax, ptFontBitMap->iYMax, ptFontBitMap->iNextOriginX, ptFontBitMap->iNextOriginY);

	return 0;
}

int FreeTypeRegister(void)
{
	return FontsRegister(&g_tFreeTypeOpr);
}

