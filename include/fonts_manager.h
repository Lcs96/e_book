#ifndef _FONTS_MANAGER_H
#define _FONTS_MANAGER_H

typedef struct FontInf{
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;
}T_FontInf, PT_FontInf;

typedef struct FontOpr{
	char * name;
	int (*FontInit)(char *pcFontFile, unsigned int size);
	int (*ShowFonts)(unsigned int uiCode, PT_FontInf ptFontInf);
}T_FontOpr, *PT_FontOpr;


int FontsInit(void);
PT_FontOpr GetFontOpr(char *pcName);
int ShowFonts(void);
int FontsRegister(PT_FontOpr ptFontOpr);
int FreeTypePut(unsigned int uiCode, PT_FontInf ptFontInf);
int FreeTypeInit(char *pcFontFile, unsigned int size);
int FreeTypeRegister(void);

#endif /*_FONTS_MANAGER_H*/