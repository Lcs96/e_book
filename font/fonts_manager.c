#include <fonts_manager.h>
#include <config.h>

static PT_FontOpr g_aptFonts[aptFontsMax];

int FontsRegister(PT_FontOpr ptFontOpr)
{
	int i;
	for (i = 0; i < aptFontsMax; i++)
		{
			if (g_aptFonts[i] == NULL)
				{
					g_aptFonts[i] = ptFontOpr;
					return 0;
				}
		}
	printf("can't register, is full\n");
	return -1;
}

int ShowFonts(void)
{
	int i;
	for (i = 0; i < aptFontsMax; i++)
		{
			if (g_aptFonts[i] != NULL)
				{
					printf("%d, %s\n",i, g_aptFonts->name);
				}
		}
	return 0;
}

PT_FontOpr GetFontOpr(char *pcName)
{
	int i;
	for (i = 0; i < aptFontsMax; i++)
		{
			if ((g_aptFonts[i] != NULL) && strcmp(g_aptFonts[i]->name, pcName))
				{
					return g_aptFonts[i];
				}
		}
	printf("can't find \n");
	return (PT_FontOpr)-1;
}

int FontsInit(void)
{
	return FreeTypeRegister();
}


















