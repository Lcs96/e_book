#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <draw.h>
#include <encoding_manager.h>
#include <fonts_manager.h>
#include <disp_manager.h>
#include <string.h>

/* ./show_file [-s Size] [-f freetype_font_file] [-h HZK] <text_file> */
int main(int argc, char **argv)
{
	unsigned int uiSize;
	char cFreetype[128];
	char cHZK[128];
	char cText[128];
	int iError;
	char cChar;
	cFreetype[0] = '\0';
	cHZK[0]      = '\0';
	cText[0]     = '\0';
	while ((iError = getopt(argc, argv, "s:f:h:")) != -1)
		{
			switch (iError)
				{
					case 's':
						uiSize = strtoul(optarg, NULL, 0);
						break;
					case 'f':
						strncpy(cFreetype, optarg, 128);
						cFreetype[127] = '\0';
						break;
					case 'h':
						strncpy(cHZK, optarg, 128);
						cHZK[127] = '\0';
						break;
					default:
						printf("Usage: %s [-s Size] [-f freetype_font_file] [-h HZK] <text_file>\n",argv[0]);
						return -1;
						break;
				}
		}
	if (optind >= argc)
		{
			printf("Usage: %s [-s Size] [-f freetype_font_file] [-h HZK] <text_file>\n",argv[0]);
			return -1;
		}
	strncpy(cText, argv[optind], 128);
	cText[127] = '\0';
	iError = DisplayInit();
	if (iError)
	{
		DBG_PRINTF("DisplayInit error!\n");
		return -1;
	}
	iError = FontsInit();
	if (iError)
	{
		DBG_PRINTF("FontsInit error!\n");
		return -1;
	}

	iError = EncodingInit();
	if (iError)
	{
		DBG_PRINTF("EncodingInit error!\n");
		return -1;
	}
		
	iError = OpenTextFile(cText);
	if (iError)
	{
		DBG_PRINTF("OpenTextFile error!\n");
		return -1;
	}
	iError = SetTextDetail(cHZK, cFreetype, uiSize);
	if (iError)
	{
		DBG_PRINTF("SetTextDetail error!\n");
		return -1;
	}
	iError = SelectAndInitDisplay("fb");
	if (iError)
	{
		DBG_PRINTF("SelectAndInitDisplay error!\n");
		return -1;
	}
	iError = ShowNextPage();
	if (iError)
	{
		DBG_PRINTF("Error to show first page\n");
		return -1;
	}
	while(1)
		{
			printf("press 'n' to next page , 'p' to pre page , 'q' to exit\n");
			do {
				cChar = getchar();
			}while ((cChar != 'n')&&(cChar != 'p')&&(cChar != 'q'));
			if (cChar == 'n')
				{	
					ShowNextPage();
				}
			else if (cChar == 'p')
				{
					ShowPrePage();
				}
			else if (cChar == 'q')
				{	
					return 0;
				}
		}
	return 0;
	
}
