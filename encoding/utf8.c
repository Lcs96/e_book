#include <encoding_manager.h>

static int IsUTF8(unsigned char *pucFileStart);
static int GetOneCodeFromFile(unsigned char *BufferStart, unsigned char *BufferEnd, unsigned int *puiCode);
static int GetPreOneBits(unsigned char *BufferStart);


static T_EncodeOpr g_tUTF8 = {
	.name        = "utf8",
	IsThisEncode = IsUTF8,
	GetOneCode   = GetOneCodeFromFile,
};

static int IsUTF8(unsigned char *pucFileStart)
{
	const char str[3] = {0xEF, 0xBB, 0xBF};
	if (strncmp((const char*)pucFileStart, str, 3) == 0)
		{
			return 1;
		}
	else
		{
			return 0;
		}
}


static int GetPreOneBits(unsigned char *BufferStart)
{
	int i;
	int iNum = 0;
	for (i = 7; i >= 0; i--)
		{
			if (*BufferStart & (0x01 << i))
				{
					iNum++;
				}
			else
				{
					return iNum;
				}
		}
	return iNum;
}

static int GetOneCodeFromFile(unsigned char *BufferStart, unsigned char *BufferEnd, unsigned int *puiCode)
{
	unsigned int iNum; 
	unsigned int uiTmp;
	int i;
	if (BufferStart >= BufferEnd)
		{
			printf("BufferStart >= BufferEnd\n");
			return -1;
		}
	iNum = GetPreOneBits(BufferStart);
	if (iNum == 0)
		{
			*puiCode = BufferStart[0];
		}
	else 
		{
			uiTmp = BufferStart[0] << 4;
			uiTmp = uiTmp >> 4;
			for (i = 1; i < iNum; i++)
				{
					BufferStart[i] = BufferStart[i] << 2;
					BufferStart[i] = BufferStart[i] >> 2;
					uiTmp = uiTmp << 6;
					uiTmp = uiTmp | BufferStart[i];
				}
			puiCode = &uiTmp;
			return 0;
		}
}

int FreeTypeRegister()
{
	g_tUTF8.aptSupportFonts[0] = GetFontOpr("freetype");
	g_tUTF8.aptSupportFonts[1] = NULL;
	return FontsRegister(&g_tUTF8);
}





